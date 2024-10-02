#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Define message structure
struct msg_buffer {
  long msg_type;
  char msg_text[100];
  int abuela;
};

int main() {
  key_t key;
  int msgid;

  // Generate unique key
  key = ftok("progfile", 65);

  // Create a message queue with read and write permissions
  msgid = msgget(key, 0666 | IPC_CREAT);

  struct msg_buffer message;

  // Example 1: Sending a message
  message.msg_type = 1;  // Message type
  strcpy(message.msg_text, "Hello, this is a message from sender");

  // Send message to the queue
  msgsnd(msgid, &message, sizeof(message.msg_text), 0);

  printf("Message sent: %s\n", message.msg_text);

  // Example 2: Receiving the message
  msgrcv(msgid, &message, sizeof(message.msg_text) + sizeof(int), 1,
         0);  // Receive message of type 1

  printf("Message received: %s\n", message.msg_text);

  // Clean up: Destroy the message queue
  msgctl(msgid, IPC_RMID, NULL);

  return 0;
}
