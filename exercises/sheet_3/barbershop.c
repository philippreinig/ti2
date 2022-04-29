/*
Aufgabe 2: Hilzer’s Barbershop-Problem [10 Pkt.]
Hilzer’s Barbershop-Problem ist eine Variante des bekannten Barbershop-Problems von Dijkstra. In diesem Szenario arbeiten drei Friseure gleichzeitig in einem Laden. Es gibt drei
Friseurstuhle, ein Sofa mit Platz für vier Kunden, einen Warteraum für weitere Kunden und
eine Kasse, welche jedoch gleichzeitig von jeweils nur einem Friseur bedient werden kann.
Des Weiteren dürfen sich Corona-bedingt insgesamt maximal zwanzig Kunden im Laden
aufhalten; weitere Kunden werden umgehend abgewiesen.
Betritt ein Kunde den Laden, so nimmt er auf dem Sofa platz und wartet, bis ein Friseur
Zeit fur ihn hat. Ist das Sofa schon besetzt, wartet er im Warteraum des Ladens. Sobald ein
Friseur frei wird, kommt ein auf dem Sofa wartender Kunde an die Reihe und ein Kunde aus
dem Warteraum nimmt auf dem Sofa Platz. Nach erfolgreichem Haarschnitt begeben sich
Kunde und Friseur zur Kasse, wo der Kunde seine Rechnung begleicht und im Gegenzug
eine Quittung erhält.
Vereinfacht sieht der Ablauf fur einen Kunden wie folgt aus:
(a) Falls weniger als 20 Kunden, so betrete den Laden
(b) Warte bis es Platz auf dem Sofa hat
(c) Warte bis ein Friseur frei ist und lasse die Haare schneiden
(d) Warte bis ein Friseur Zeit hat um zu bezahlen
(e) Verlasse den Laden
Analog gilt fur den Friseur: ¨
(a) Warte bis ein Kunde bereit ist
(b) Scheide ihm die Haare
(c) Warte bis die Kasse frei ist
(d) Warte bis der Kunde bezahlt hat
In unserem Beispiel gibt es keine strikte Bindung von Kunden zu ihrem Friseur. Es kann
also sein, dass ein Kunde von Friseur A die Haare geschnitten bekommt, aber bei Friseur B
bezahlt.
Programmieren Sie, basierend auf dem bereitgestellten Template barbershop.c, eine korrekte Umsetzung des beschriebenen Barbershop-Problems mit Hilfe von Semaphoren. L¨osen
Sie dazu die unten aufgeführten Teilaufgaben und überprüfen Sie deren Korrektheit anhand der Textausgabe.
(a) Maximale Kundenanzahl [2 T.Pkt.]
Stellen Sie sicher, dass sich maximal zwanzig Kunden im Laden befinden (Tipp: Critical
section)
(b) Sofa [2 T.Pkt.]
Fügen Sie das Sofa hinzu (Tipp: Auf vier initialisierte Semaphore)
(c) Haareschneiden [2 T.Pkt.]
Implementieren Sie das Haareschneiden (Tipp: Rendez-Vous)
(d) Bezahlvorgang [2 T.Pkt.]
Fügen Sie den Bezahlvorgang hinzu (Tipp: Critical section, Rendez-Vous)
(e) Optional: Erweitertes Warten [2 Z.Pkt.]
Falls sich bereits zwanzig Leute im Laden befinden, verschwinden neue Kunden nicht auf
Nimmerwiedersehen, sondern gehen ein Getränk Ihrer Wahl trinken und versuchen es anschliessend nochmals. Das Trinken des Getränks kann mit der Funktion sleep() modelliert
werden.
(f) Optional: GUI [2 Z.Pkt.]
Erstellen Sie eine ansprechende GUI z.B. in C, welche den Laden, das Mobiliar, die Friseure
und die Kunden entsprechend des Programmablaufs darstellt.

Weitere Bewertung:
Das Programm kompiliert [1 T.Pkt.]
Das Programm ist fehlerfrei [1 T.Pkt.]

*/
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Datastructure to pass thread arguments
 */
struct shop_thread_priv_data;

/**
 * @brief Datastructure representing the barberstore
 */
struct shop {
  /**
   * @brief Binary semaphore protecting access to customers_curr
   */
  sem_t customers_in_shop;
  /**
   * @brief Max amount of customers simultaneously in the shop
   */
  int customers_shop_max;
  /**
   * @brief Amount of customers currently in the shop
   */
  int customers_curr;
  /**
   * @brief Semaphore counting free seats on the sofa
   */
  sem_t sofa;
  /**
   * @brief Binary semaphore protecting access to the cashdesk
   */
  sem_t cashdesk;
  /**
   * @brief Counting semaphore used to indicate a customer is ready to receive a cut
   */
  sem_t cut_customer_ready;
  /**
   * @brief Counting semaphore used to indicate a barber is ready to cut
   */
  sem_t cut_barber_ready;
  /**
   * @brief Counting semaphore used to indicate a cut is done
   *
   */
  sem_t cut_done;
  /**
   * @brief Counting semaphore used to indicate a customer is ready to pay
   */
  sem_t pay_customer_ready;
  /**
   * @brief Counting semaphore used to indicate a barber is ready receive the pay
   */
  sem_t pay_barber_ready;
  /**
   * @brief Array of barber thread ids
   */
  pthread_t *barbers;
  /**
   * @brief Array of arguments for barber threads
   */
  struct shop_thread_priv_data *barbers_priv;
  /**
   * @brief Amount of available barbers
   */
  int barbers_num;
  /**
   * @brief Array of customer thread ids
   */
  pthread_t *customers;
  /**
   * @brief Array of arguments for customer threads
   */
  struct shop_thread_priv_data *customers_priv;
  /**
   * @brief Total amount of customers trying to enter the shop
   */
  int customers_num;
};

struct shop_thread_priv_data {
  /**
   * @brief Thread number
   */
  int thread_num;

  /**
   * @brief Pointer to shop instance
   */
  struct shop *shop;
};

/**
 * @brief Prints a messages and exists with failure code
 *
 * @param serror Error message
 */
static void panic(const char *serror) {
  fprintf(stderr, "%s", serror);
  exit(EXIT_FAILURE);
}

/**
 * @brief Allocates memory and panics on allocation error
 *
 * @param size Size in bytes to allocate
 * @return void* Pointer to allocated memory
 */
static void* xmalloc(size_t size) {
  void *ptr;
  if (size == 0) {
    panic("Size is 0!\n");
  }
  ptr = malloc(size);
  if (!ptr) {
    panic("No mem left!\n");
  }
  return ptr;
}

/**
 * @brief Frees allocated memory space at an address returned by xmalloc
 *
 * @param ptr Pointer to allocated memory space
 */
static void xfree(void *ptr) {
  if (ptr == NULL) {
    panic("Got Null-Pointer!\n");
  }
  free(ptr);
}

/**
 * @brief Handler for receiving a signal. Exits the currently running thread with 0
 *
 * @param signum Received signal. Ignored in this handler
 */
void signal_quit(int signum) {
  pthread_exit(0);
}

/**
 * @brief Thread function for barbers
 *
 * @param arg Pointer to barber thread arguments
 * @return void* nothing
 */
static void* barber_main(void *arg) {
  struct shop *s;
  struct shop_thread_priv_data *std = arg;

  printf("barber %d alive\n", std->thread_num);
  s = std->shop;
  signal(SIGQUIT, signal_quit);

  /* Do the barber job */
  while (1) {
    /* I want to cut hair */
    sem_post(&s->cut_barber_ready);
    /* I am waiting for a client */
    printf("barber %d waiting for customer\n", std->thread_num);
    sem_wait(&s->cut_customer_ready);
    /* I am cutting hair */
    printf("barber %d cutting hair\n", std->thread_num);
    sleep(3);
    /* Tell customer we're done */
    printf("barber %d finished hair cut\n", std->thread_num);
    sem_post(&s->cut_done);
    /* ... */
    /* Wait for cash desk until free */
    printf("barber %d waiting until cash desk is free\n", std->thread_num);

    sem_wait(&s->cashdesk);
    printf("barber %d went to cash desk, waits for customer to pay\n", std->thread_num);
    /* Make the payment */
    sem_post(&s->pay_barber_ready);
    sem_wait(&s->pay_customer_ready);
    printf("barber %d got payment\n", std->thread_num);
    /* Free cash desk */
    sem_post(&s->cashdesk);
  }

  printf("barber %d died\n", std->thread_num);
  pthread_exit(0);
}

/**
 * @brief Thread function for customers
 *
 * @param arg Pointer to customer thread arguments
 * @return void* nothing
 */
static void* customer_main(void *arg) {
  struct shop *s;
  struct shop_thread_priv_data *std = arg;

  printf("customer %d alive\n", std->thread_num);
  s = std->shop;

  /* Only enter if customers_curr < customers_shop_max, else go grab a drink and try again later */
  int shop_entered = 0;
  while(!shop_entered){
    sem_wait(&s->customers_in_shop);
      printf("Customer %d trying to enter shop.\n", std->thread_num);
    if(s->customers_curr>=s->customers_shop_max){
      printf("Customer %d won't enter shop, because there are currently %d/%d customers inside. Grabbing a drink instead.\n", std->thread_num, s->customers_curr, s->customers_shop_max);
      sem_post(&s->customers_in_shop);
      /* Sleeping for 5s to model grabbing a drink */
      sleep(5);
    }else{
      printf("Customer %d is going to enter the shop, because there are only %d/%d customers inside.\n", std->thread_num, s->customers_curr, s->customers_shop_max);
      shop_entered = 1;
      ++s->customers_curr;
      sem_post(&s->customers_in_shop);
    }
  }

  printf("customer %d entered shop, waiting for a spot on sofa\n", std->thread_num);


  /* Wait on the sofa */
  sem_wait(&s->sofa);
  printf("customer %d sitting on sofa, waits for barber\n", std->thread_num);

  /* Wait for a barber */
  sem_post(&s->cut_customer_ready);
  sem_wait(&s->cut_barber_ready); 

  /* Leave the sofa */
  sem_post(&s->sofa);
  printf("customer %d went up from sofa\n", std->thread_num);

  /* Wait until barber is done */
 
  sem_wait(&s->cut_done);

  /* Pay for the hair cut */
  sem_post(&s->pay_customer_ready);
  sem_wait(&s->pay_barber_ready);
  printf("customer %d payed money\n", std->thread_num);

  /* Leave the shop */
  sem_wait(&s->customers_in_shop);
  --s->customers_curr;
  sem_post(&s->customers_in_shop);

  printf("customer %d left the shop, %d still in the shop\n", std->thread_num, s->customers_curr);
  printf("customer %d died\n", std->thread_num);
  pthread_exit(0);
}

/**
 * @brief Initialise the shop and its fields
 *
 * @param s Pointer to the shop object.
 */
static void shop_create(struct shop *s) {
  s->barbers = xmalloc(s->barbers_num * sizeof(*(s->barbers)));
  s->barbers_priv = xmalloc(s->barbers_num * sizeof(*(s->barbers_priv)));
  s->customers = xmalloc(s->customers_num * sizeof(*(s->customers)));
  s->customers_priv = xmalloc(s->customers_num * sizeof(*(s->customers_priv)));

  sem_init(&s->customers_in_shop, 0, 1);
  sem_init(&s->sofa, 0, 4);
  sem_init(&s->cashdesk, 0, 1);

  sem_init(&s->cut_customer_ready, 0, 0);
  sem_init(&s->cut_barber_ready, 0, 0);
  sem_init(&s->cut_done, 0, 0);
  sem_init(&s->pay_customer_ready, 0, 0);
  sem_init(&s->pay_barber_ready, 0, 0);
}

/**
 * @brief Start all customer and barber threads
 *
 * @param s Pointer to the shop
 */
static void shop_run(struct shop *s) {
  int i, ret;

  for (i = 0; i < s->barbers_num; ++i) {
    s->barbers_priv[i].thread_num = i;
    s->barbers_priv[i].shop = s;
    ret = pthread_create(&s->barbers[i], NULL, barber_main, &s->barbers_priv[i]);
    if (ret < 0) {
      panic("Cannot create thread!\n");
    }
  }

  for (i = 0; i < s->customers_num; ++i) {
    s->customers_priv[i].thread_num = i;
    s->customers_priv[i].shop = s;
    ret = pthread_create(&s->customers[i], NULL, customer_main, &s->customers_priv[i]);
    if (ret < 0) {
      panic("Cannot create thread!\n");
    }
  }
}

/**
 * @brief Waits for all processes to end and cleans up allocated memory and semaphores
 *
 * @param s Pointer to the shop
 */
static void shop_cleanup(struct shop *s) {
  int i;

  for (i = 0; i < s->customers_num; ++i) {
    pthread_join(s->customers[i], NULL);
  }
  for (i = 0; i < s->barbers_num; ++i) {
    pthread_kill(s->barbers[i], SIGQUIT);
  }
  for (i = 0; i < s->barbers_num; ++i) {
    pthread_join(s->barbers[i], NULL);
  }

  xfree(s->customers);
  xfree(s->customers_priv);
  xfree(s->barbers);
  xfree(s->barbers_priv);

  sem_destroy(&s->customers_in_shop);
  sem_destroy(&s->sofa);
  sem_destroy(&s->cashdesk);

  sem_destroy(&s->cut_customer_ready);
  sem_destroy(&s->cut_barber_ready);
  sem_destroy(&s->cut_done);
  sem_destroy(&s->pay_customer_ready);
  sem_destroy(&s->pay_barber_ready);
}

int main(int argc, char **argv) {
  struct shop s;

  memset(&s, 0, sizeof(s));
  s.barbers_num = 5;
  s.customers_num = 25;
  s.customers_shop_max = 20;

  shop_create(&s);
  shop_run(&s);
  shop_cleanup(&s);

  return 0;
}
