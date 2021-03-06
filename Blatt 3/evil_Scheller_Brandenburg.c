#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// https://www.linuxprogrammingblog.com/code-examples/sigaction wurde als Verständnishilfe
// für die grundlegende Funktionalität von sigaction verwendet.

// Die Handler-Funktion
void endlos() {

  // Ein timespec struct (das zeitgebende Argument für nanosleep) wird deklariert.
  struct timespec printzeit;

  // Die abzulaufende Zeit des timespecs wird auf 2 Sekunden gestellt.
  printzeit.tv_sec = 2;

  // Die PID des Prozesses wird ausgegeben Anmerkung: Ja, hier mit printf,
  //da wir keinen einfachen Weg gefunden haben einen Int mit write auszugeben.
  printf("[Hier könnte Ihre böse Nachricht stehen!]\nPID: %d\n", getpid());

  while (1) {
    // Ein nanosleep mit einer Länge von 2 Sekunden wird durchgeführt.
    nanosleep(&printzeit, NULL);
    char str[] = "Endlosschleife\n";
    write(1,str, strlen(str));
  }

}

int main(int argc, char const *argv[]) {

  //silence compiler
  (void) argc;
  (void) argv[0];

  // Ein timespec (das zeitgebende Argument für nanosleep) wird deklariert.
  struct timespec zeit;

  // Die abzulaufende Zeit des timespecs wird auf 1 Sekunde gestellt.
  zeit.tv_sec = 1;

  // Ein sigaction struct wird deklariert.
  struct sigaction action;

  // endlos() wird als die Handler-Funktion der sigaction festgelegt.
  action.sa_handler = &endlos;

  // Die sigaction wird aufgerufen. Sie soll hier auf SIGINT oder SIGTERM reagieren und ruft, wie in action deklariert,
  // beim Auftreten dieses Signals die endlos() Funktion auf.
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  // Das Programm wird in eine Endlosschleife versetzt, damit es nicht sofort abbricht.
  // Nicht besonders elegant, aber naja.
  while (1) {
    nanosleep(&zeit, NULL);
  }
}
