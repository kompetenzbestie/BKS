#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char const *argv[]) {
  unsigned int s;
  struct sockaddr_un remote;
  int len, t;
  char buf[100];

  // Es wird überprüft, ob ein Pfad angegeben wurde.
  if (argc < 2) {
    perror("Please provide a path.");
    exit(1);
  } else if (argc > 2) {
    perror("Unexpected arguments. Please provide only one path.");
    exit(1);
  }

  // Ein Stream-Socket wird geöffnet.
  s = socket(AF_UNIX, SOCK_STREAM, 0);

  // Der Pfad wird aus den angegeben Argumenten erzeugt.
  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, argv[1]);

  len = strlen(remote.sun_path) + sizeof(remote.sun_family);

  // Der Client versucht nun, eine Verbindung zu dem angegebenen Socket aufzubauen.
  if(connect(s, (struct sockaddr *)&remote, len) == -1) {
    perror("Connection failed");
    exit(1);
  }

  printf("Connected.\n");

  // Der Client öffnet nun einen Eingabeprompt.
  while (printf("> "), fgets(buf, 100, stdin), !feof(stdin)) {
    // Wenn eine Eingabe erfolgt ist, versucht der Client diese über den Socket an den Server zu schicken.
    if (send(s, buf, strlen(buf),0) == -1) {
      perror("Unable to send messages to server");
      exit(1);
    }

    // Der Client wartet nun auf eine Antwort vom Server.
    if ((t=recv(s, buf, 100, 0)) > 0) {
      // Wenn eine Antwort (i.d.R. "OK") erhalten wurde, wird dieser ein null character angefügt und sie wird ausgegeben.
      buf[t] = '\0';
      printf("server> %s\n", buf);
    } else {
      // Wenn keine Antwort zurückkommt, wird überprüft, ob der Fehler an der recv()-Funktion liegt oder ob der Server die Verbinfung geschlossen hat.
      // Im zweiten Fall wird dieser Umstand in der Ausgabe reflektiert, der Client schließt den Socket und terminiert.
      if (t < 0) perror("recv");
      else printf("Server closed connection\n");

      close(s);
      exit(1);
    }
  }

  close(s);
  return 0;
}

// QUELLE: http://beej.us/guide/bgipc/html/multi/unixsock.html (18.06.2020, 18:23 Uhr)
