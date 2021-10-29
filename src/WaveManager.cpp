#include "WaveManager.h"
#include "Enemy.h"
#include <stdexcept>
#include <time.h>
#include <cstdlib>

WaveManager::WaveManager()
{
    currentWave = 0;
    enemyCount = 0;
    aliveEnemyCount =0;
}

bool WaveManager::waveOver()
{
    for(int i=0; i<enemyCount; i++)
    {
        if(enemies.at(i)->getIsAlive())
        {
            return(false);
        }
    }
    return(true);
}

void WaveManager::beginWave(sf::Vector2<float> player)
{
    currentWave++;
    // For now, waves will progress linearly for simple demonstration sake
    enemyCount = currentWave;
    // Spawn enemies
    Enemy* temp = nullptr;
    for(int i=0; i<enemyCount; i++)
    {
        temp = new Enemy();
        sf::Vector2<float> spawn;
        bool loop;
        spawn = sf::Vector2<float> (std::rand()%600, std::rand()%300);
        /*do {
            std::srand(time(0));
            // TODO IN MERGE: change max coords
            spawn = sf::Vector2<float> (std::rand()%600, std::rand()%300);
            if((spawn.x-player.x < 100 && spawn.x-player.x > -100) || (spawn.y-player.y < 100 && spawn.y-player.y > -100))
            {
                loop = true;
            }
            else
            {
                loop = false;
                for(int j=0; j<i; j++)
                {
                    // TODO MAYBE: change distances here?
                    if((spawn.x-enemies.at(j)->getPosition().x < 30 && spawn.x-enemies.at(j)->getPosition().x > -30) || (spawn.y-enemies.at(j)->getPosition().y < 30 && spawn.y-enemies.at(j)->getPosition().y > -30))
                    {
                        loop = true;
                        break;
                    }
                }
            }
        }while(loop);*/
        temp->spawn(spawn); 
        enemies.push_back(temp);
    }
}

void WaveManager::endWave()
{
    // Clear gamestate
    while(enemies.size() > 0)
    {
        delete enemies.at(enemies.size()-1);
        enemies.pop_back();
    }
}

int WaveManager::getWave()
{
    return(currentWave);
}

int WaveManager::getEnemies()
{
    return(enemyCount);
}

int WaveManager::getEnemiesRemaining()
{
    int alive = 0;
    for(int i=0; i<enemyCount; i++)
    {
        if(enemies.at(i)->getIsAlive())
        {
            alive++;
        }
    }
    return(alive);
}

void WaveManager::updateWaves(sf::Vector2<float> player)
{
    if(waveOver())
    {
        endWave();
        beginWave(player);
    }
    else {}
}

void WaveManager::updateAliveEnemyCount()
{
    aliveEnemyCount = getEnemiesRemaining();
}

void WaveManager::updateEnemies(float time, sf::Vector2<float> player)
{
    for(int i=0; i<enemyCount; i++)
    {
        enemies.at(i)->update(time, player);
    }
}

void WaveManager::waveDraw()
{
    for(int i=0; i<enemyCount; i++)
    {
        enemies.at(i)->onDraw();
    }
}

Enemy* WaveManager::getEnemy(int n)
{
    // Blah blah not how blah blah no enemies blah blah
    if(n<(int)enemies.size())
    {
        return(enemies.at(n));
    }
    // no clue how this is gonna work with enemies, god I wish we used rust
    //return(-1);
    throw std::runtime_error("literally how");
}