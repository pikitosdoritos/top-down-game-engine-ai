#include "Scenes/GameScene.hpp"
#include "Entities/Player.hpp"
#include "Entities/Enemy.hpp"
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

// Declared in GameOverScene.cpp — written here so GameScene can set it
extern bool g_playerWon;

using namespace engine;

// ── Helpers ──────────────────────────────────────────────────────────────────

static Vec2f overlap(const FloatRect& a, const FloatRect& b)
{
    float ax1 = a.position.x, ay1 = a.position.y;
    float ax2 = ax1 + a.size.x, ay2 = ay1 + a.size.y;
    float bx1 = b.position.x, by1 = b.position.y;
    float bx2 = bx1 + b.size.x, by2 = by1 + b.size.y;

    float ox = std::min(ax2, bx2) - std::max(ax1, bx1);
    float oy = std::min(ay2, by2) - std::max(ay1, by1);
    if (ox <= 0.f || oy <= 0.f) return {0.f, 0.f};
    return {ox, oy};
}

// ── Scene lifecycle ───────────────────────────────────────────────────────────

void GameScene::onEnter(engine::GameEngine& engine)
{
    m_killCount    = 0;
    m_totalEnemies = 0;
    m_playerDead   = false;
    m_victory      = false;
    m_endTimer     = 0.f;
    m_entities.clear();
    m_player = nullptr;

    // Load font
    auto& res = engine.resources();
    const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");

    // Build tilemap — procedural room, no texture required
    engine::TilesetInfo ts;
    ts.texture    = nullptr;
    ts.tileWidth  = 32;
    ts.tileHeight = 32;
    ts.columns    = 1;
    m_tilemap.buildRoom(30, 22, 32, 32, ts, 0, 1);

    // Camera
    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());
    m_camera = Camera({W, H});
    m_camera.setBounds(m_tilemap.worldBounds());

    // Spawn player and enemies
    spawnPlayer(engine);
    spawnEnemies(engine);
    m_totalEnemies = m_killCount = 0; // killCount starts 0, totalEnemies set in spawnEnemies
    // recalculate total
    for (auto& e : m_entities)
        if (e->tag() == "enemy") ++m_totalEnemies;

    // HUD
    float barW = 200.f, barH = 18.f;
    m_playerHPBar.emplace(sf::Vector2f{16.f, H - 40.f},
                           sf::Vector2f{barW, barH},
                           sf::Color(200, 30, 30),
                           sf::Color(50, 20, 20),
                           sf::Color::Black);

    m_hpLabel.emplace(font, "HP", 16, sf::Color(220, 200, 180));
    m_hpLabel->setPosition({16.f, H - 60.f});

    m_killLabel.emplace(font, "Kills: 0", 18, sf::Color(180, 160, 100));
    m_killLabel->setPosition({W - 130.f, 14.f});
}

void GameScene::onExit(engine::GameEngine& /*engine*/)
{
    m_entities.clear();
    m_player = nullptr;
    m_playerHPBar.reset();
    m_hpLabel.reset();
    m_killLabel.reset();
}

void GameScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
{
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>())
        if (kp->code == sf::Keyboard::Key::Escape)
            engine.quit();
}

// ── Spawn helpers ─────────────────────────────────────────────────────────────

void GameScene::spawnPlayer(engine::GameEngine& /*engine*/)
{
    auto p = std::make_unique<Player>();
    m_player = p.get();

    auto* tf = m_player->getComponent<TransformComponent>();
    if (tf) tf->position = {3 * 32.f + 16.f, 3 * 32.f + 16.f}; // near top-left inside walls

    m_entities.push_back(std::move(p));
}

void GameScene::spawnEnemies(engine::GameEngine& /*engine*/)
{
    // Spawn positions spread across the dungeon room
    const std::vector<Vec2f> spawnPoints = {
        {22 * 32.f, 3  * 32.f},
        {25 * 32.f, 10 * 32.f},
        {22 * 32.f, 18 * 32.f},
        {15 * 32.f, 15 * 32.f},
        {15 * 32.f, 5  * 32.f},
        {10 * 32.f, 12 * 32.f},
        {5  * 32.f, 15 * 32.f},
        {5  * 32.f, 8  * 32.f},
    };

    for (const auto& pos : spawnPoints) {
        auto enemy = std::make_unique<Enemy>(pos, 60.f);

        // Give each enemy its attack callback via AIComponent.behaviourUpdate
        // Capture m_player as raw pointer — valid for lifetime of GameScene
        Enemy* rawEnemy = enemy.get();
        Player* rawPlayer = m_player;

        auto* ai = enemy->getComponent<AIComponent>();
        ai->behaviourUpdate = [rawPlayer, rawEnemy](
                Entity& /*self*/, GameEngine& /*engine*/, float /*dt*/)
        {
            auto* selfAI = rawEnemy->getComponent<AIComponent>();
            if (!selfAI || selfAI->state != AIState::Attack) return;
            if (selfAI->attackTimer > 0.f) return;

            if (auto* hp = rawPlayer->getComponent<HealthComponent>()) {
                hp->takeDamage(selfAI->attackDamage);
            }
            selfAI->attackTimer = selfAI->attackCooldown;
        };

        m_entities.push_back(std::move(enemy));
    }
}

// ── Update ────────────────────────────────────────────────────────────────────

void GameScene::update(engine::GameEngine& engine, float dt)
{
    if (m_playerDead || m_victory) {
        m_endTimer -= dt;
        if (m_endTimer <= 0.f) {
            g_playerWon = m_victory;
            engine.scenes().switchTo(SceneID::GameOver);
        }
        return;
    }

    // Feed AI the player's current world position
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

    // Update all entities (sets velocities, fires attacks etc.)
    for (auto& e : m_entities) {
        if (e->active && !e->destroyed)
            e->update(engine, dt);
    }

    // Drain projectile spawns from Player
    if (m_player) {
        for (auto& proj : m_player->pendingSpawns)
            m_entities.push_back(std::move(proj));
        m_player->pendingSpawns.clear();
    }

    // Apply melee hitbox — check enemies against player attack box this frame
    if (m_player && m_player->isAttacking()) {
        FloatRect hit = m_player->attackHitbox();
        for (auto& e : m_entities) {
            if (e->tag() != "enemy" || e->destroyed) continue;
            auto* etf = e->getComponent<TransformComponent>();
            auto* ecol = e->getComponent<ColliderComponent>();
            if (!etf || !ecol) continue;
            FloatRect eb = ecol->worldBounds(etf->position);
            auto ov = overlap(hit, eb);
            if (ov.x > 0.f && ov.y > 0.f) {
                if (auto* hp = e->getComponent<HealthComponent>())
                    hp->takeDamage(m_player->attackDamage);
            }
        }
    }

    // Entity-vs-entity collision (projectile triggers + enemy/player push)
    m_collision.process(m_entities);

    // Tile wall collision — push any non-static entity out of solid tiles
    handleTileCollisions();

    // Camera follows player
    if (m_player) {
        auto* ptf = m_player->getComponent<TransformComponent>();
        if (ptf) m_camera.follow(ptf->position, dt, 6.f);
    }

    // HUD update
    if (m_player) {
        auto* hp = m_player->getComponent<HealthComponent>();
        if (hp && m_playerHPBar) m_playerHPBar->setRatio(hp->ratio());
    }

    // Kill counter
    int liveEnemies = 0;
    for (auto& e : m_entities)
        if (e->tag() == "enemy" && !e->destroyed)
            ++liveEnemies;
    m_killCount = m_totalEnemies - liveEnemies;

    if (m_killLabel) {
        m_killLabel->setString("Kills: " + std::to_string(m_killCount) +
                               " / " + std::to_string(m_totalEnemies));
    }

    // Win / lose conditions
    if (m_player) {
        auto* hp = m_player->getComponent<HealthComponent>();
        if (hp && hp->isDead() && !m_playerDead) {
            m_playerDead = true;
            m_victory    = false;
            m_endTimer   = 2.2f;
        }
    }
    if (!m_playerDead && liveEnemies == 0 && m_totalEnemies > 0) {
        m_victory  = true;
        m_endTimer = 2.5f;
    }

    pruneDeadEntities();
}

// ── Tile collision ────────────────────────────────────────────────────────────

void GameScene::handleTileCollisions()
{
    const auto& solidRects = m_tilemap.solidRects();

    for (auto& e : m_entities) {
        if (!e->active || e->destroyed) continue;
        if (e->tag() == "projectile")  continue; // projectiles pass through walls in this demo

        auto* tf  = e->getComponent<TransformComponent>();
        auto* col = e->getComponent<ColliderComponent>();
        if (!tf || !col || col->isTrigger) continue;

        FloatRect eBounds = col->worldBounds(tf->position);

        for (const auto& tile : solidRects) {
            auto ov = overlap(eBounds, tile);
            if (ov.x == 0.f && ov.y == 0.f) continue;

            // Push along axis of least penetration
            if (ov.x < ov.y) {
                float sign = (tf->position.x < tile.position.x + tile.size.x * 0.5f) ? -1.f : 1.f;
                tf->position.x += sign * ov.x;
            } else {
                float sign = (tf->position.y < tile.position.y + tile.size.y * 0.5f) ? -1.f : 1.f;
                tf->position.y += sign * ov.y;
            }

            // Recompute after push
            eBounds = col->worldBounds(tf->position);
        }
    }
}

// ── Render ────────────────────────────────────────────────────────────────────

void GameScene::render(engine::GameEngine& engine)
{
    auto* win = engine.renderer().window();

    // World view — apply camera
    m_camera.apply(*win);
    m_tilemap.draw(*win);

    // Draw entities back-to-front (simple y-sort)
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

    for (auto* e : drawList) e->render(*win);

    // Melee swing arc (visual feedback)
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

    // UI view — switch to screen-space
    engine.renderer().setUIView();

    if (m_hpLabel)    m_hpLabel->draw(*win);
    if (m_playerHPBar) m_playerHPBar->draw(*win);
    if (m_killLabel)  m_killLabel->draw(*win);

    // Overlay on win / lose
    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());

    if (m_playerDead || m_victory) {
        // Semi-transparent overlay
        sf::RectangleShape overlay({W, H});
        overlay.setFillColor(sf::Color(0, 0, 0, 120));
        win->draw(overlay);
    }
}

// ── Cleanup ───────────────────────────────────────────────────────────────────

void GameScene::pruneDeadEntities()
{
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
            [](const std::unique_ptr<engine::Entity>& e) { return e->destroyed; }),
        m_entities.end()
    );
}
