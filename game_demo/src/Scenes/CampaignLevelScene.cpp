#include "Scenes/CampaignLevelScene.hpp"
#include "Campaign/CampaignState.hpp"
#include "Entities/Player.hpp"
#include "Entities/Enemy.hpp"
#include "Entities/Brute.hpp"
#include "Entities/Projectile.hpp"

#include "engine/Core/GameEngine.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include "engine/Components/HealthComponent.hpp"
#include "engine/Components/ColliderComponent.hpp"
#include "engine/Components/AIComponent.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace engine;

// ── Helpers ────────────────────────────────────────────────────────────────

static Vec2f aabbOverlap(const FloatRect& a, const FloatRect& b)
{
    float ox = std::min(a.position.x + a.size.x, b.position.x + b.size.x)
             - std::max(a.position.x, b.position.x);
    float oy = std::min(a.position.y + a.size.y, b.position.y + b.size.y)
             - std::max(a.position.y, b.position.y);
    if (ox <= 0.f || oy <= 0.f) return {0.f, 0.f};
    return {ox, oy};
}

// Generates a pseudorandom spawn point safely inside the map bounds on a floor tile.
static Vec2f safeSpawn(const Tilemap& tm, int cols, int rows, int tileSize, int seed)
{
    srand(static_cast<unsigned>(seed * 7919 + 1337));
    int x = 0, y = 0;
    int tries = 0;
    while (tries < 100) {
        x = rand() % cols;
        y = rand() % rows;
        if (!tm.isSolid(x, y)) break;
        tries++;
    }
    return {(x + 0.5f) * tileSize, (y + 0.5f) * tileSize};
}

// ── Lifecycle ──────────────────────────────────────────────────────────────

void CampaignLevelScene::onEnter(engine::GameEngine& engine)
{
    const LevelConfig& cfg = CampaignState::current();

    m_killCount    = 0;
    m_totalEnemies = 0;
    m_playerDead   = false;
    m_victory      = false;
    m_endTimer     = 0.f;
    m_entities.clear();
    m_player = nullptr;

    m_ambientColor = sf::Color(cfg.ambR, cfg.ambG, cfg.ambB);

    auto& res = engine.resources();
    const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");

    // Textures — load if available, fail silently
    try { res.loadTexture("player",    "assets/textures/player.png"); }  catch(...) {}
    try { res.loadTexture("enemy",     "assets/textures/enemy.png");  }  catch(...) {}
    try { res.loadTexture("brute",     "assets/textures/brute.png");  }  catch(...) {}
    try { res.loadTexture("sword",     "assets/textures/sword.png");  }  catch(...) {}
    try { res.loadTexture("bullet",    "assets/textures/bullet.png"); }  catch(...) {}
    try { res.loadTexture("current_tileset", "assets/textures/" + cfg.tilesetAsset + ".png"); } catch(...) {}

    buildMap(engine);
    spawnPlayer(engine);
    spawnEnemies(engine);

    // Count enemies
    for (auto& e : m_entities)
        if (e->tag() == "enemy") ++m_totalEnemies;

    // Camera
    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());
    m_camera = Camera({W, H});
    m_camera.setBounds(m_tilemap.worldBounds());

    // Fog of war
    m_fog.init(cfg.mapCols, cfg.mapRows, cfg.tileSize, cfg.tileSize);

    // HUD ─────────────────────────────────────────────────────────────────
    // Level name — top left
    m_levelLabel.emplace(font, cfg.levelName, 20, sf::Color(220, 200, 140));
    m_levelLabel->setPosition({16.f, 14.f});

    // Objective — below level name
    m_objectiveLabel.emplace(font, cfg.objectiveText, 16, sf::Color(160, 160, 100));
    m_objectiveLabel->setPosition({16.f, 40.f});

    // Kill counter — below objective
    m_killLabel.emplace(font, "Kills: 0 / 0", 16, sf::Color(180, 140, 80));
    m_killLabel->setPosition({16.f, 62.f});

    // HP bar — bottom left
    float barW = 200.f, barH = 18.f;
    m_hpBar.emplace(sf::Vector2f{16.f, H - 40.f},
                    sf::Vector2f{barW, barH},
                    sf::Color(200, 30, 30),
                    sf::Color(50, 20, 20),
                    sf::Color::Black);

    m_hpLabel.emplace(font, "HP", 16, sf::Color(220, 200, 180));
    m_hpLabel->setPosition({16.f, H - 60.f});
}

void CampaignLevelScene::onExit(engine::GameEngine& /*engine*/)
{
    m_entities.clear();
    m_player = nullptr;
    m_hpBar.reset();
    m_hpLabel.reset();
    m_levelLabel.reset();
    m_objectiveLabel.reset();
    m_killLabel.reset();
    m_transitionLabel.reset();
}

void CampaignLevelScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
{
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Escape)
            engine.quit();
    }
}

// ── Update ────────────────────────────────────────────────────────────────

void CampaignLevelScene::update(engine::GameEngine& engine, float dt)
{
    const LevelConfig& cfg = CampaignState::current();

    // End-state countdown
    if (m_playerDead || m_victory) {
        m_endTimer -= dt;
        if (m_endTimer <= 0.f) {
            if (m_victory) {
                // Advance campaign level counter before loading next scene
                CampaignState::get().currentLevel++;
                engine.scenes().switchTo(cfg.nextScene);
            } else {
                // On death: reset to level 1, return to menu
                CampaignState::get().currentLevel = 1;
                engine.scenes().switchTo(SceneID::Menu);
            }
        }
        return;
    }

    // Push camera view to player for correct mouse aiming
    if (m_player) m_player->setCameraView(m_camera.sfView());

    // Feed AI the player position
    if (m_player) {
        auto* ptf = m_player->getComponent<TransformComponent>();
        if (ptf) {
            for (auto& e : m_entities) {
                if (e->tag() == "enemy") {
                    if (auto* ai = e->getComponent<AIComponent>())
                        ai->targetPosition = ptf->position;
                }
            }
        }
    }

    // Update all entities
    for (auto& e : m_entities)
        if (e->active && !e->destroyed)
            e->update(engine, dt);

    // Drain player projectile spawns
    if (m_player) {
        for (auto& proj : m_player->pendingSpawns)
            m_entities.push_back(std::move(proj));
        m_player->pendingSpawns.clear();
    }

    // Melee hitbox check
    if (m_player && m_player->isAttacking()) {
        FloatRect hit = m_player->attackHitbox();
        for (auto& e : m_entities) {
            if (e->tag() != "enemy" || e->destroyed) continue;
            auto* etf  = e->getComponent<TransformComponent>();
            auto* ecol = e->getComponent<ColliderComponent>();
            if (!etf || !ecol) continue;
            auto ov = aabbOverlap(hit, ecol->worldBounds(etf->position));
            if (ov.x > 0.f && ov.y > 0.f)
                if (auto* hp = e->getComponent<HealthComponent>())
                    hp->takeDamage(m_player->attackDamage);
        }
    }

    m_collision.process(m_entities);
    handleTileCollisions();

    // Camera follow
    if (m_player) {
        auto* ptf = m_player->getComponent<TransformComponent>();
        if (ptf) {
            m_camera.follow(ptf->position, dt, 5.f);
            // Fog reveal around player (180 px radius)
            m_fog.reveal(ptf->position, 180.f);
        }
    }

    // HUD update
    if (m_player) {
        auto* hp = m_player->getComponent<HealthComponent>();
        if (hp && m_hpBar) m_hpBar->setRatio(hp->ratio());
    }

    int liveEnemies = 0;
    for (auto& e : m_entities)
        if (e->tag() == "enemy" && !e->destroyed) ++liveEnemies;
    m_killCount = m_totalEnemies - liveEnemies;
    if (m_killLabel)
        m_killLabel->setString("Kills: " + std::to_string(m_killCount)
                               + " / " + std::to_string(m_totalEnemies));

    // Win/lose check
    if (m_player) {
        auto* hp = m_player->getComponent<HealthComponent>();
        if (hp && hp->isDead() && !m_playerDead) {
            m_playerDead = true;
            m_endTimer   = 2.5f;
        }
    }
    if (!m_playerDead && liveEnemies == 0 && m_totalEnemies > 0) {
        m_victory  = true;
        m_endTimer = 3.0f;
        // Show transition label
        if (!m_transitionLabel) {
            auto& res = engine.resources();
            const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");
            float W = static_cast<float>(engine.window().width());
            float H = static_cast<float>(engine.window().height());
            std::string msg = (cfg.nextScene == SceneID::CampaignVictory)
                ? "CAMPAIGN COMPLETE!"
                : "LEVEL COMPLETE!  Preparing next area...";
            m_transitionLabel.emplace(font, msg, 30, sf::Color(220, 200, 60));
            m_transitionLabel->centerX(W);
            m_transitionLabel->setPosition(
                {m_transitionLabel->sfText()->getPosition().x, H * 0.42f});
        }
    }

    pruneDeadEntities();
}

// ── Tile collision ─────────────────────────────────────────────────────────

void CampaignLevelScene::handleTileCollisions()
{
    for (auto& e : m_entities) {
        if (!e->active || e->destroyed) continue;

        auto* tf  = e->getComponent<TransformComponent>();
        auto* col = e->getComponent<ColliderComponent>();
        if (!tf || !col) continue;
        if (col->isTrigger && e->tag() != "projectile") continue;

        FloatRect eBounds = col->worldBounds(tf->position);

        for (const auto& tile : m_tilemap.solidRects()) {
            auto ov = aabbOverlap(eBounds, tile);
            if (ov.x == 0.f && ov.y == 0.f) continue;

            if (e->tag() == "projectile") {
                e->destroyed = true;
                break;
            }

            if (ov.x < ov.y) {
                float sign = (tf->position.x < tile.position.x + tile.size.x * 0.5f) ? -1.f : 1.f;
                tf->position.x += sign * ov.x;
            } else {
                float sign = (tf->position.y < tile.position.y + tile.size.y * 0.5f) ? -1.f : 1.f;
                tf->position.y += sign * ov.y;
            }
            eBounds = col->worldBounds(tf->position);
        }
    }
}

// ── Render ─────────────────────────────────────────────────────────────────

void CampaignLevelScene::render(engine::GameEngine& engine)
{
    auto* win = engine.renderer().window();

    // Set custom ambient clear colour (already set in GameEngine::render, but
    // override via draw of a full-screen rect before tilemap — simpler)
    win->clear(m_ambientColor);

    // World view
    m_camera.apply(*win);
    m_tilemap.draw(*win);

    // Fog of war overlay — drawn after tilemap, before entities
    m_fog.draw(*win);

    // Y-sorted entity draw
    std::vector<engine::Entity*> drawList;
    drawList.reserve(m_entities.size());
    for (auto& e : m_entities)
        if (e->active && !e->destroyed)
            drawList.push_back(e.get());

    std::sort(drawList.begin(), drawList.end(), [](engine::Entity* a, engine::Entity* b) {
        auto* ta = a->getComponent<engine::TransformComponent>();
        auto* tb = b->getComponent<engine::TransformComponent>();
        float ya = ta ? ta->position.y : 0.f;
        float yb = tb ? tb->position.y : 0.f;
        return ya < yb;
    });

    for (auto* e : drawList) {
        // Skip rendering non-player entities if hidden in Fog
        if (e != m_player) {
            auto* tf = e->getComponent<engine::TransformComponent>();
            if (tf && !m_fog.isVisible(tf->position)) continue;
        }
        e->render(*win);
    }

    // Melee swing arc
    if (m_player && m_player->isAttacking()) {
        FloatRect hb = m_player->attackHitbox();
        sf::RectangleShape arc;
        arc.setSize({hb.size.x, hb.size.y});
        arc.setOrigin({hb.size.x * 0.5f, hb.size.y * 0.5f});
        arc.setPosition({hb.position.x + hb.size.x * 0.5f,
                         hb.position.y + hb.size.y * 0.5f});
        arc.setFillColor(sf::Color(255, 220, 80, 60));
        arc.setOutlineColor(sf::Color(255, 240, 120, 120));
        arc.setOutlineThickness(1.5f);
        win->draw(arc);
    }

    // UI (screen-space)
    engine.renderer().setUIView();

    if (m_levelLabel)    m_levelLabel->draw(*win);
    if (m_objectiveLabel) m_objectiveLabel->draw(*win);
    if (m_killLabel)     m_killLabel->draw(*win);
    if (m_hpLabel)       m_hpLabel->draw(*win);
    if (m_hpBar)         m_hpBar->draw(*win);
    if (m_transitionLabel) m_transitionLabel->draw(*win);

    // Death / victory overlay
    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());
    if (m_playerDead) {
        sf::RectangleShape overlay({W, H});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        win->draw(overlay);
    }
}

// ── Spawn helpers ──────────────────────────────────────────────────────────

void CampaignLevelScene::buildMap(engine::GameEngine& engine)
{
    const LevelConfig& cfg = CampaignState::current();
    auto& res = engine.resources();

    TilesetInfo ts;
    ts.texture    = res.getTexture("current_tileset");
    ts.tileWidth  = cfg.tileSize;
    ts.tileHeight = cfg.tileSize;
    ts.columns    = 1; // Tileset is 32x64 or 32x128 (1 column)

    m_tilemap.buildDungeon(cfg.mapCols, cfg.mapRows,
                           cfg.tileSize, cfg.tileSize, ts, 0, 1);
}

void CampaignLevelScene::spawnPlayer(engine::GameEngine& engine)
{
    const LevelConfig& cfg = CampaignState::current();
    const sf::Texture* ptex = engine.resources().getTexture("player");
    const sf::Texture* swordTex = engine.resources().getTexture("sword");

    auto p = std::make_unique<Player>(ptex, swordTex);
    m_player = p.get();

    auto* tf = m_player->getComponent<TransformComponent>();
    if (tf) {
        // Find first floor tile for safe spawn
        bool found = false;
        for (int y = 5; y < cfg.mapRows && !found; ++y) {
            for (int x = 5; x < cfg.mapCols && !found; ++x) {
                // We'll just pick the first room-like space
                // (better would be to pick center of first room but this is fast)
                float px = (x + 0.5f) * cfg.tileSize;
                float py = (y + 0.5f) * cfg.tileSize;
                
                // Check if this pixel is on a floor tile (collision 0)
                int tx = static_cast<int>(px / cfg.tileSize);
                int ty = static_cast<int>(py / cfg.tileSize);
                // Tilemap doesn't expose collision grid directly but we can assume buildDungeon works
                // Just use first room from buildDungeon logic or simple check:
                // Actually let's just use the safeSpawn logic with a seed
                tf->position = safeSpawn(m_tilemap, cfg.mapCols, cfg.mapRows, cfg.tileSize, 12345);
                found = true;
            }
        }
    }
    m_entities.push_back(std::move(p));
}

void CampaignLevelScene::attachAttackCallback(engine::Entity& enemy, float /*damage*/)
{
    auto* ai = enemy.getComponent<AIComponent>();
    if (!ai || !m_player) return;

    engine::Entity* rawEnemy  = &enemy;
    Player*         rawPlayer = m_player;

    ai->behaviourUpdate = [rawPlayer, rawEnemy](
            Entity& /*self*/, GameEngine& /*engine*/, float /*dt*/)
    {
        auto* selfAI = rawEnemy->getComponent<AIComponent>();
        if (!selfAI || selfAI->state != AIState::Attack) return;
        if (selfAI->attackTimer > 0.f) return;
        if (auto* hp = rawPlayer->getComponent<HealthComponent>())
            hp->takeDamage(selfAI->attackDamage);
        selfAI->attackTimer = selfAI->attackCooldown;
    };
}

void CampaignLevelScene::spawnEnemies(engine::GameEngine& engine)
{
    const LevelConfig& cfg = CampaignState::current();
    const sf::Texture* etex = engine.resources().getTexture("enemy");

    // Grunts
    for (int i = 0; i < cfg.gruntCount; ++i) {
        Vec2f pos = safeSpawn(m_tilemap, cfg.mapCols, cfg.mapRows, cfg.tileSize,
                               i * 311 + 5000 + CampaignState::get().currentLevel * 100);

        auto enemy = std::make_unique<Enemy>(pos, cfg.gruntHP, etex);
        auto* ai = enemy->getComponent<AIComponent>();
        ai->moveSpeed   = cfg.gruntSpeed;
        ai->attackDamage = cfg.gruntDamage;
        ai->hasLineOfSight = [this](Vec2f from, Vec2f to) {
            return m_tilemap.hasLineOfSight(from, to);
        };
        attachAttackCallback(*enemy, cfg.gruntDamage);
        m_entities.push_back(std::move(enemy));
    }

    // Brutes (level 2+)
    const sf::Texture* btex = engine.resources().getTexture("brute");
    for (int i = 0; i < cfg.bruteCount; ++i) {
        Vec2f pos = safeSpawn(m_tilemap, cfg.mapCols, cfg.mapRows, cfg.tileSize,
                               i * 537 + 9000 + CampaignState::get().currentLevel * 200);

        auto brute = std::make_unique<Brute>(pos,
            cfg.bruteHP, cfg.bruteSpeed, cfg.bruteDamage, btex);
        auto* bai = brute->getComponent<AIComponent>();
        if (bai) {
            bai->hasLineOfSight = [this](Vec2f from, Vec2f to) {
                return m_tilemap.hasLineOfSight(from, to);
            };
        }
        attachAttackCallback(*brute, cfg.bruteDamage);
        m_entities.push_back(std::move(brute));
    }
}

void CampaignLevelScene::pruneDeadEntities()
{
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
            [](const std::unique_ptr<engine::Entity>& e){ return e->destroyed; }),
        m_entities.end());
}
