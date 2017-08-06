# SoftWar
Game Project For ETNA Alternance in C

**Server**

/server/
    include/
    libmy/
    model/
    src/
        server/
        tools/

***Build et usages***

****Build****

au niveau /server/*

$ make
$ ./sw_server [option]

****Options****

! = paramètre obligatoire

Logs: -v et -log
    sans -v, les erreurs sont loggées dans /serveur/error.log.
    -v seul: log en INFO sortie console.
    -v [error|warning|info|debug] debug affiche tout, error seulement les erreurs, etc.
    -log ![path/to/file] relatif à /server/*, ne crée pas les répertoires si il y en a.

Port d'écoute du REQ/ROUTER:
    -rep-port ![int > 0], par défaut sur 4242

Port de diffusion du PUB/SUB:
    -pub-port ![int > 0], par défaut sur 4243

Définition du thread de publication:
    -cycle ![int > 0], par défaut sur 1 000 000 microsec

Taille de la map : 
    -map-size ![2 < int < 16], par défaut la map est de type: 4x4

Une fois lancé, le server va initialisé ses sockets et les mettre à l'écoute dans une zpoll.
Le client fournis envoit une requête identify|identity à la connection. Le serveur gère.
L'input doit être de type zmsg_t (pour le client, j'ai vus des lib en nodejs pour ca).
Sa composition en frame doit être la suivante: address|commande|identity
Soit: 
    zmsg_t = zmsg_new()
    zmsg_pushstr(identity); 
    zmsg_pushstr(commande);

La commande doit être de la forme: 'commande_valide'|paramètre_optionnel

Le serveur hash le nom des commandes au premier appel sur le serveur.
Le hashage fonctionne simplement:
    t_command **commands[hash_command("identify")] permet de récupérer la fonction identify().

Le serveur gère: identify, selfid, et leave, 6 autres commandes sont prêtes à être définies.

Il ne compte pas les players et délivre des uid auto-incrémentée (0x01, 0x02 etc) tant qu'il tourne
et sans vérification particulière. En revanche, quand il y a 4 joueurs, le t_game_manager possède un
bollean (manager->ready) qui passe à 1. Ca peut servir comme début d'implémentation d'un status start/stop.

Lors du process, on stocke dans le contexte: la socket active, l'id du player actif mais pas encore l'adresse.
Avoir le message en morceau serait pratique mais pas indispensable.

**Client**
Gros client de test bien bourrin, tout est dans le main...
/client_rep/
    include/
    libmy/
    src/
        tools/

***Build et usages***

****Build****

au niveau /server/*

$ make
$ ./demo [option]

****Options****

! = paramètre obligatoire

Logs: -v et -log
    sans -v, les erreurs sont loggées dans /serveur/error.log.
    -v seul: log en INFO sortie console.
    -v [error|warning|info|debug] debug affiche tout, error seulement les erreurs, etc.
    -log ![path/to/file] relatif à /server/*, ne crée pas les répertoires si il y en a.

Une fois lancé, ce faux client de test envoit une première requête "identify|identity".
A la réponse du serveur il récupère son uid. Par la suite, il donne la possibilité d'envoyer
des requêtes REQ/ROUTER au server.

Il bind automatiquement les ports 4242 et 4243.
