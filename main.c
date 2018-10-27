#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int tiempo;
unsigned char *s, *t, *flag, *j, *result;

void payload() {
  int payload_size, i;
  
  unsigned char s_payload[] = {
    0x91, 0x87, 0xee, 0xec, 0x2c, 0x25, 0x21, 0x0e,
    0x1b, 0xa5, 0xc0, 0x9c, 0x2e, 0x42, 0xfd, 0xbf,
    0x93, 0x96, 0xc1, 0xce, 0xc6, 0xdf, 0x0
  };

  unsigned char t_payload[] = {
    0x74, 0x5a, 0x1b, 0x67, 0xde, 0x34, 0xf6, 0x34,
    0x67, 0x5a, 0x8b, 0x74, 0x5a, 0x1b, 0x67, 0xde,
    0x34, 0xf6, 0x34, 0x67, 0x5a, 0x8b, 0x31, 0x3b,
    0x36, 0x30, 0x2c, 0x03, 0x3f, 0x66, 0x24, 0x08,
    0x66, 0x24, 0x08, 0x39, 0x67, 0x23, 0x08, 0x24,
    0x36, 0x39, 0x64, 0x2a, 0x0
  };

  /* Initialize flag */
  printf("[debug] Initializing flag, size=%ld\n", 0x00000000000041df - 0x00000000000041a0);
  flag = malloc(0x00000000000041df - 0x00000000000041a0);

  printf("[debug] flag initialized\n");
  
  /* Initialize s */
  printf("[debug] Initializing s, size=%ld\n", 0x00000000000040bf - 0x0000000000004080);
  s = malloc(0x00000000000040bf - 0x0000000000004080);

  payload_size = strlen(s_payload);
  printf("[debug] s payload size: %d\n", payload_size);

  for (i = 0; i < payload_size; i++) {
    s[i] = s_payload[i];
  }

  printf("[debug] s initialized\n");

  /* Initialize t */
  t = malloc(0x000000000000413f - 0x00000000000040c0);
  printf("[debug] Initializing t, size=%ld\n", 0x000000000000413f - 0x00000000000040c0);

  payload_size = strlen(t_payload);

  printf("[debug] t payload size: %d\n", payload_size);

  for (i = 0; i < payload_size; i++) {
    t[i] = t_payload[i];
  }

  printf("[debug] t initialized\n");

  /* Initialized j */
  printf("[debug] Initializing j, size=%ld\n", 0x000000000000419f - 0x0000000000004160);
  j = malloc(0x000000000000419f - 0x0000000000004160);
  printf("[debug] j initialized\n");
}

void *timer(void * arg) {
  goto loc_12d4;

 loc_12d4:
  usleep(0xf4240); // sleep 1s
  tiempo = tiempo + 1;
  goto loc_12d4;
}

void *tr1(void * arg) {
  int var_14 = 0;

  while ( var_14 < strlen(flag)) {
    j[var_14] = strlen(flag) ^ flag[var_14] & 255;
    usleep(0x30d40); // slep 2s
    var_14++;
  }
}

void *tr2(void * arg) {
  int var_14 = 0;
  while (var_14 < strlen(flag)) {
    usleep(0x61a80); // sleep 4s
    j[var_14] = t[2 * tiempo] & 255 ^ (j[var_14] & 255) ^ 0xffffff80;
    var_14++;
  }
}

void dump() {
  int i;

  printf("[Debug] flag dump: ");
  for (i = 0; i < 13; i++) {
    printf("%x ", flag[i]);
  }
  printf("\n");

  printf("[Debug] j dump: ");
  for (i = 0; i < 63; i++) {
    printf("%x ", j[i]);
  }
  printf("\n");

}

/**
 * Ingenieria reversa a s.
 */

void *tr1_d(void * arg) {
  int var_14 = 0;

  while ( var_14 < strlen(s)) {
    result[var_14] = strlen(s) ^ s[var_14];
    usleep(0x30d40); // slep 2s
    var_14++;
  }
}

void *tr2_d(void * arg) {
  int var_14 = 0;
  while (var_14 < strlen(s)) {
    usleep(0x61a80); // sleep 4s

    result[var_14] = t[2 * tiempo] ^ result[var_14];
    result[var_14] ^= 0xffffff80;
    var_14++;
  }
}


void decode() {
  int i;
  pthread_t var_8, var_10, var_18;

  result = malloc(strlen(s));
  
  pthread_create(&var_8, 0x0, &timer, 0x0);
  pthread_create(&var_10, 0x0, &tr1_d, 0x0);
  pthread_create(&var_18, 0x0, &tr2_d, 0x0);

  printf("[debug] Sleeping %d seconds\n", 13);
  usleep(13000000);

  for (i = 0; i < strlen(s); i++) {
    printf("%c", result[i]);
  }
}


int main(int argc, char** args) {
  payload();
  decode();
  return;
  
  /** <start>original **/
  pthread_t var_8, var_10, var_18;

  puts("Introduce flag: ");
  scanf("%s", flag);
  /**
   * Escribe 12 en el vector tiempo en la primera columna.
   **/
  
  pthread_create(&var_8, 0x0, &timer, 0x0);
  pthread_create(&var_10, 0x0, &tr1, 0x0);
  pthread_create(&var_18, 0x0, &tr2, 0x0);

  printf("[debug] Sleeping %d seconds\n", 13);
  usleep(13000000);

  if (strcmp(j, s) == 0x0) {
    printf("la flag es: %s\n Enhorabuena !!! \n", flag);
  }
  else {
    puts("Me parece que te faltan unas cruzcampo");
  }
  
  /** <end>original **/
  dump();
}
