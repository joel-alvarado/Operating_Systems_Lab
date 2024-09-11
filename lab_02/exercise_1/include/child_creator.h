#ifndef CHILD_CREATOR_H
#define CHILD_CREATOR_H
#include <sys/types.h>

/**
 * @brief Spawn 3 forks of the current process
 *
 */
void CreateChildren();

/**
 * @brief Spawns 2 fork from current process and turns them into greetings
 * binary
 *
 */
void SpawnGreetingsForks();

/**
 * @brief Prints to stdout the PID of the current process
 *
 */
void PrintPid();

/**
 * @brief Gets the directory for the current executable and puts it in binPath
 *
 * @param binPath
 * @param size
 */
void GetExecutablePath(char* binPath, size_t size);

/**
 * @brief Runs chmod syscall with permission 0755 on binPath
 *
 * @param binPath
 */
void AllowExecution(char* binPath);

#endif /* CHILD_CREATOR_H */