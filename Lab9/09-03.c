#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/sem.h>

/**
 * Синхронизация:
 * Отец выполняет операцию D(1), чтобы не дать ребёнку помешаем ему прочитать сообщение и написать новое.
 * А рёбнок выполняет при новой итерации цикла операцию Z, то есть на проверку равенста нулю, а потом увеличивает
 * Значение семафора на 2. Чтобы вновь стало 0, нужно, чтобы отец зашёл в цикл и вышел, то есть два раза
 * декриментнул.
 */
void try_semop(int sem_id, int sem_op) {
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_flg = 0;
  mybuf.sem_op  = sem_op;

  if (semop(sem_id, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}

/**
 *  Просто увеличиваем значение
 */
void A(int sem_id, int value) {
  try_semop(sem_id, value);
}

/**
 * Уменьшаем значение и ждём >= 0
 */ 
void D(int sem_id, int value) {
  try_semop(sem_id, -value);
}

/**
 * Проверка на 0.
 */
void Z(int sem_id) {
  try_semop(sem_id, 0);
}


int main()
{
    // Инициализируем: 
    int parent[2], result;
    size_t size = 0;
    key_t key;
    char pathname[] = "05-3.c";
    struct sembuf buffer;

    int semid;
    char resstring[14];

    // Создаём Pipes родителя и ребёнка:
    if (pipe(parent) < 0) {
        printf("Невозможно создать pipe отца\n\r");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Ключ не сгенерирован.\n");
        exit(-1);
    }

    // Создаём семафорчик
    if ((semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
        printf("Can\'t create semaphore set\n");
        exit(-1);
        } else if ((semid = semget(key, 1, 0)) < 0) {
        printf("Can\'t find semaphore\n");
        exit(-1);
        }
    } else {
        A(semid, 2);
    }

    int N;
    printf("Введите N: \n");
    scanf("%d", &N);
    if (N < 2) {
        printf("N должен быть больше или равен 2.\n");
        exit(-1);
    }


    result = fork();

    if (result < 0) {
        printf("Неудачный fork\n\r");
        exit(-1);
    }

    // Отец:
    else if (result > 0) {
        for (size_t i = 0; i < N; i++)
        {
            D(semid, 1);
            if (i != 0) {
                // Читаем с ребёнка.
                size = read(parent[0], resstring, 14);
                if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
                }
                printf("%d. Parent read message:%s\n", i, resstring);
            }

            // Пишем сообщение ребёнку.
            size = write(parent[1], "Hello, world!", 14);
            
            if (size != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            }
            D(semid, 1);
        }
        close(parent[0]);
    }
    else {
        // Ребёнок
        int counter = 0;
        // Читаем отца:
        for (int i = 0; i < N; ++i) {
            Z(semid);

            // Читаем сообщение отца
            size = read(parent[0], resstring, 14);
            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }
            printf("%d. Child read message:%s\n", ++counter, resstring);

            // Пишем сообщение отцу
            size = write(parent[1], "Hello, parent", 14);
            if (size != 14) {
                printf("Can\'t write all string to pipe: %d\n", size);
                exit(-1);
            }

            A(semid, 2);
        }

        close(parent[1]);
        close(parent[0]);
    }
    return 0;
}
