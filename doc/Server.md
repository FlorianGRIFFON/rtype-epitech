# :file_cabinet: Server

---

The server is the heart of our project, the server contains different functionalities such as :

- [**ECS (Entity Component System) architecture**](# ECS (Entity Component System) )
- [**GameEngine**](# Game-Engine)
- [**Server**](# Server)

## ECS (Entity Component System)

The Entity Component System (ECS) is a software design architecture commonly used in video game development.

- **Entity**: An entity is a key element in the ECS. It represents an object where we can characterise it as a character, an object or an enemy in a game. We can associate different components to define its characteristics and behaviour.
- **Component**: Components are data that are designed to provide the attributes and properties of an entity. For example, a position component will store the coordinates of an entity. Components are modular and can be reused to create different entities.
- **System**: Systems are modules of code that will afir on the logic and manipulation of entities according to their component. Each system handles a specific task, such as managing artificial intelligence.

![Entity Component System, Data Locality, C IndieGameDev](https://indiegamedev.net/wp-content/uploads/2020/05/ecs.svg)

---

## GameEngine

The GameEngine will allow you to create players (`playerEntity`), enemys (`enemyMosquito`) and projectiles (`Entity projectile`). The GameEngine will also update the players' lives, positions, etc.

Here is the function that creates a player:

```c++
void createPlayer(struct GameStruct *gameStruct, Coordinator& gCoordinator, std::string name, udp::endpoint endpoint)
{
    Entity entity = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(entity, PositionComponent(getRandomPos()));
    gCoordinator.AddComponent(entity, SpriteComponent(Vec2(66.0f, 34.0f), 1, gameStruct->entityID, true));
    gCoordinator.AddComponent(entity, PlayerComponent(name, endpoint));
    gCoordinator.AddComponent(entity, LifeComponent(3));

    gameStruct->entityID++;

    endpointToEntityMap[endpoint] = entity;
}
```

Here's a function that makes this change:

```c++
void DamageSystem::TakeDamage(int damage, Entity entity)
{
    auto& life = gCoordinator.GetComponent<LifeComponent>(entity);

    life.remainingLife -= damage;

    if (life.remainingLife <= 0) {
        auto& sprite = gCoordinator.GetComponent<SpriteComponent>(entity);
        sprite.isAlive = false;
    }
}
```

If you want to see more click on this [folder](path to folder).

---

## Server

The server is the heart of the project. It was created using the [UDP](https://fr.wikipedia.org/wiki/User_Datagram_Protocol) protocol (`User Datagram protocol`).
 data from a source host to a destination host, without checking whether the destination host has received all the data.

![What Is User Datagram ProtocolUDP? What are its benefits?](https://bunnyacademy.b-cdn.net/F7AJp-What-Is-UDP-how-does-it-work-and-what-are-its-benefits.png)

To manage the flow of data, we used [Multithreading](https://www.geeksforgeeks.org/multithreading-in-cpp/) in C++, a technique that allows our program to run several threads (processing units) simultaneously, thus improving performance. This allows tasks to be performed in parallel, but requires synchronisation to avoid data conflicts.

### How do you create an entity?

1. Create a `createEntityName` function and give it `(struct GameStruct* gameStruct, Coordinator& gCoordinator)` as parameters.

2. Define the function in config.hpp

3. Still in config.hpp, create an entity vector, as shown here
   std::vector<Entity> enemiesNameVector;`

4. Then complete the `create()` function.

5. Create an entity with `gCoordinator.CreateEntity()`.
   Then give it **components**. For example, position, sprite and its life.

6. Then give it **components**. For example, position, sprite and its life.
   
   ```
   In order for the client to know what to display, and where, each entity must have a `SpriteComponent` and a `PositionComponent`.
   
   cpp
   gCoordinator.AddComponent(enemyPlane, SpriteComponent(Vec2(50.0f, 50.0f), 3, gameStruct->entityID, true));
   ```
   
   Here, for example, the `enemyPlane` has an `spriteCode` of 3, which tells the client to display sprite number 3 (see section on codes and sprites).

7. Once all this is done, we need to push the created entity into the corresponding vector.
   
   ```cpp`
   gameStruct->nameVector.push_back(entityName);
   
   ```
   
   ```

8. We have created an entity, and it is now present in the vector for which it was created.

9. Maintenant, afin de ne pas avoir de problèmes de lecture et écriture, il faut lui créer un mutex. Dans le `Mutex.cpp` et `Mutex.hpp`, créez simplement un mutex du nom de votre entité.

10. Finally, we now need to send the information from these entities to the 
    client. To do this, simply add these lines to the `sendOperation.cpp` :

```c++
{
    std::unique_lock<std::mutex> entityLock(entityMutex);
    for (auto it = gameStruct->entityNameVector.begin(); it != gameStruct->entityNameVector.end();) {
        const Entity entity = *it;

        if (!sendEntityData(entity, socket)) {
            std::cout << "Destroying entity directly - Name" << std.endl;
            gCoordinator.DestroyEntity(entity);
            it = gameStruct->entityNameVector.erase(it); // Remove the entity from the vector
        } else {
            ++it;
        }
    }
}
```

11. And that's it, you're now sending the information for each entity in the vector! Before each `createEntity`, you need to lock your mutex. Do a function like 
    
    void createEntityNameSafely(struct GameStruct *gameStruct) {
     // Lock the mutex to ensure exclusive access to the entity vector.
     std::lock_guard<std::mutex> lock(entityMutex);
     createEntityName(gameStruct, gCoordinator);

}

12. You can now make your entities perform specified actions. In the `MovementSystem`, you have a `UpdateEnemies` function.
    In it, create your motion logic and apply it to each entity in your vector. 
    each entity in your vector. Don't forget to use the mutex.

13. You can also create a new system and write your code logic in it, using the `MovementSystem` model. Don't forget to **register** this system and call it in the `callFunctionPeriodically` function in `server.cpp`.

---
