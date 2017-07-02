*Utilisation des libraries extended:*

	**Liblist:**

	***Structures: ***
	
	t_link: {
		int 	index;
		void	*content;
		t_link	*next;
		t_link	*prev;
	}

	t_chain: {
		int	index;
		t_link  *first;
		t_link	*last;
		t_link	**dictionnary;
		void	( *free )( t_chain **chain );
	}

	****Fonctions à usage Public:****
	
	***--------------------------libmy/chain_manager.c----------------------------***

	t_chain	*create_chain( void ( *free )(t_chain** ) );

		Prend une fonction free en paramètre ou NULL.
		Retourne une t_chain malloc. Doit être libérée avec
		la fonction delete_chain.

	int		delete_chain(t_chain **chain);
	
		Supprime et libère la chain passée en paramètre.
		Si le pointeur *free est set, delete_chain l'appelle,
		libérant le contenu de la manière souhaitée par
		l'utilisateur de la lib.
		Retourne 1 si erreur, 0 si ok.	
	
	Int		add_link( t_chain **chain, void *content );

		Prend la chaine et un pointeur sur void en paramètres.
		Retourne 1 si erreur, 0 si ok.
		Lie le content à la chain. 
		Le nouveau link devient le last.
		Le first est toujours le même.

	Int		remove_link( t_chain **chain, t_link *link);

		Retire et libère le link passé en paramètre.
		Si le t_link n'appartient pas à la chaîne,
		il est quand même free.

	Int		chain_is_null(t_chain **chain);

		Check if chain and (*chain) is NULL.
		Retourne 1 si erreur, 0 si ok.

	Int		chain_is_empty(t_chain **chain);

		Check if chain, (*chain) and (*chain)->first is NULL
		Retourne 1 si erreur, 0 si ok.
	
	-------------------------Libmy/get_chain.c------------------------	

	t_link		*get_link_by_index( int index, t_chain *chain );

		Permet d'accéder directement à un maillon de la chaine
		en passant par son index, comme avec un array.
		Il s'agit simplement d'un appel au code suivant:
		return chain->dictionnary[index];

	t_link		*get_link_by_content( void *content, t_chain *chain );

		Useless. 


	Fonctions à usage privée:

		Les fonctions liées à l'indexation sont utilisées de manière
		transparentes pour l'utilisateur de la lib. Les fonctions utiles
		lors du développement sont celles décrite plus haut.

	------------------------libmy/indexor.c --------------------------

	int		init_index( t_chain **chain );

		Initialise l'index de la liste chainée.
		Appellée lors d'un create_chain.
		Retourne 1 si erreur, 0 si ok.

	Int		add_to_index( t_chain **chain, t_link **link );

		Ajoute le nouveau link à l'indexation de la chaine.
		Retourne 1 si erreur, 0 si ok.

	Int		remove_from_index( t_chain **chain, t_link *link);

		Retire le link de l'indexation.
		Retourne 1 si erreur, 0 si ok.




Libparse:

	La libparse permet l'ajout d'options au programme et la gestion du bon format des arguments passés par l'utilisateur. Pour ce faire, la lib se base sur des paramètres fournis par l'utilisateur lors de la construction des options.
	La libparse facilite l'exécution des fonctions liées aux options en organisant les paramètres dans une t_chain contenu par l'option. Il reste à l'utilisateur de gérer les différents ordres possibles dans les arguments passés par l'utilisateur ainsi que les casts nécessaires à l'appel de sa propre fonction de callback.

Structures: 

	t_option {
		int		index;
		int		mandatory;
		int		mandatory_arguments;
		int		allowed_arguments;
		int		to_execute;
		char		*name;
		void		*action;
		t_chain	*parameters;
	}

index:  inutilisé mais peut servir d'id ou d'incrément.

mandatory: bollean, 1 pour une option obligatoire, 0 pour optionnelle
	         les constantes MANDATORY et OPTIONNAL sont définies
	         dans libmy.h.

mandatory_arguments: à utiliser comme un int. 
			      définis le nombre d'arguments obligatoires à passer à l'option. 
			      Si l'utilisateur ne fournis pas le bon nombre d'arguments 
			      la fonction parse() renverra 1. 

allowed_arguments: à utiliser comme un int.
			 définis le nombre total d'arguments acceptés par l'option.
			 Exemples:
				Si l'utilisateur crée une option avec 2 arguments obligatoires
			et définis 0 arguments autorisés, il ne se passe rien.
			
				Si l'utilisateur définis 2 arguments obligatoire et 3 arguments
			autorisés, le programme cherchera la présence d'un 3ème optionnel,
			mais parse() ne renverra pas 1 si ce 3ème argument est absent.

to_execute: bollean, passe à 1 dans le processus de parsing des arguments si l'option est
	         présente et valide. Permet à l'utilisateur de la lib de déclencher la fonction de
	         callback facilement sur les bonnes options.

name: nom de l'option. Sert à la comparaison avec des appels à my_strcmp pour l'appel de la 	
fonction de callback.
	La lib se base sur la présence de l'ASCII 45: '-' pour déterminer la présence d'une option.
	Ainsi, une option commençant par - - sera prise en compte. 
action: pointeur sur void. Permet de stocker la fonction de callback. Doit être castée lors de l'appel.

parameters:  Une fois l'input de l'utilisateur validé et l'option parsée, les arguments envoyés par l'utilisateur sont rangés dans la liste chainée 'parameters'. L'utilisateur de la lib reçoit les arguments rangés dans l'ordre dans lequel ils ont étés renseignés et doit se charger des casts éventuels et de la gestion d'erreur.

Fonctions publiques:
	Gestion des options:

	------------------------ libmy/options_manager.c --------------------------

	
	  t_option		new_option( int mandatory,
					          int mandatory_arguments,
					          int allowed_arguments,
					          char *name,
					          void *action)

	
	void		free_options_in_chain(t_chain **chain);

		Fonction à passer à la creation de la t_chain options.
		Il est nécessaire que les options soient rangées dans cette
		structure pour que les fonctions de parsing fonctionnent.
		Du coup, on aura toujours la même structure à libèrer.

		Exemple de définition de l'option help, faisant appel là une fonction
		'void display_help();' qui affiche les commande avec des my_putstr, 
		si l'utilisateur appelle le programme avec -h.
		Pas d'arguments, pas de retour à cette fonction de callback:

	t_chain	*get_options()
	{
		t_chain  *options;
		t_option *opt_help;

		if ((opt_help = new_option( 0, 0, 0, "-h", display_help)) == NULL)
			return (NULL);
		if ((options = create_chain(free_options_in_chain)) == NULL)
			return (NULL);
		if (add_link(&options, opt_help))
			return (NULL);
		return (options);
	}

		L'option help est optionnelle, ne prend pas d'arguments obligatoires,
		n'a pas d'arguments autorisés et déclenche display_help() après la réussite de la 		
		fonction parse sur:

		(void (*)(void))((t_option*)((t_link*)options->first)->content)->action)()

		ou:

		t_link 		ltmp;
		t_option	otmp;
		
		ltmp = options->first;
		otmp = ltmp->content;
		(void (*) void)(otmp->action)();

		
	--------------------------------- libmy/parse.c ---------------------------

	int		parse( int begin, int argc, char *argv[], t_chain **options );

		Simplement utilisées pour appeller validate_opt() et vérifié le retour.
		Begin indique l'index à partir duquel le parsing doit commencer.
		Permet d'évacuer ./a.out.		

	Fonction privées, processus de parsing:

	--------------------------------- libmy/validate_opt.c ---------------------------

	int		validate_opt( int begin, int argc, char *argv[], t_chain **options );

		Boucle sur les argv puis sur les options. Chaque option est comparée
		à chaque argv. Sur [0] == 45, on lance le traitement d'une option.
		Appel check_arguments() et checkmandatories() pour réaliser le parsing.
		Retourne 1 si erreur, 0 si ok.

	Int	check_arguments( int index_argv, int argc, t_option *option, char *argv[] );

		Effectue les différentes vérifications entre les attendus de l'option et 
		l'input fournis.
		Fait appel à format_parameters pour ranger les arguments 
		dans la t_chain parameters.
		Retourne 1 si erreur 0 si ok.

	Int	format_parameters(int pos, int parameters, char *argv[], t_option **option);

		Construit la t_chain dans option->parameters. Cette t_chain est malloc dans
		new_option(). Donc ici on ne soucis que d'ajouter des links.

	Int	check_mandatories( int argc, char *argv[], t_chain **options);

		V�rifie les indicateurs mandatory et display les messages d'erreurs.
Liblog:

	Structures: 
	
	t_logger: {
		int 	lvl;
		char	*level;
		void	(*log)(const char*, char*, int);
		char	*file_path;
	}

	Static:

		log_panel: tableau de pointeurs sur fonctions. Les fonctions sont définies dans
		libmy/log_panel.c et sont des templates pour display les niveaux de log.

	4 niveaux de log possibles: 1 = ERROR, 2 = WARNING, 3 = INFO et 4 = DEBUG.
	Deux fonctions pour display des logs: 
void my_log(const char*__func__, char *message, int lvl);
void devlog(const char*__func__, char *message, int lvl);
	
	Si my_log est appellé avant d'avoir appellé la fonction build_logger, le logger est définis par défaut au niveau ERROR et écrivant dans le fichier error.log. Tout les my_log définis avecc un lvl > 1 ne seront pas retranscris.

	Soit build_logger est appellé manuellement dans le code, mais la conception correspond plutôt à un fonctionnement de pair avec libparse:

	En définissant une option sur -v avec un paramètre optionnel, l'utilisateur de la lib permet à l'utilisateur du programme de:

définir le logger sur INFO sans paramètres
définir le logger sur le niveau de son choix avec le paramètre:
debug|info|warning|error
exemple: ./a.out -v warning ou ./a.out -v

	En définissant une option -log avec un paramètre obligatoire, l'utilisateur de la lib définis un fichier de son choix pour receptionner les logs.
	Exemple d'un appel complet pour une retranscription des logs en WARNING dans le fichier warning.log:

	./a.out -v warning -log warning.log

	Si devlog est appellé, qu'importe si build_logger a été appellé, un logger temporaire est malloc et free durant devlog. Ce logger log en DEBUG dans un fichier dev.log.
Il permet d'envoyer des logs pendant les fonctions d'exécutions des options, lorsque le logger static n'est pas encore instancié.


	----------------------------------- libmy/logger.c --------------------------------
	
	Fonctions publiques:
	
	void			my_log(const char* __func__, char *message, int lvl);

		1er paramètre; la constante __func__ permet d'afficher le nom de la
		fonction dans laquelle my_log est appellé.
		2ème paramètre: le message d'erreur, sans \n ou autre formatage.
		3ème paramètre: le niveau de log, 1, 2, 3 ou 4 (erreur, warning, info ou debug).
		Appelle get_logger().
		Par défaut et sans appel personnalisé de build_logger, my_log loggera par défaut 		
		les my_log(__func__, "message", 1); dans error.log. Aucun lvl > 1 ne passera.
		

	void			devlog(const char* __func__, char *message, int lvl);

		Version développement de my_log. Instancie et free dans chaque appel
		� devlog, un logger ERROR >> error.log.

	t_logger		*build_logger(char *opt, t_chain *parameters);

		Cette fonction est Appellée dans 2 contextes différents: 

			soit dans le cadre d'une option, elle prend alors le nom
			de l'option (-v ou -log) et les paramètres parsés par la libparse.

			Soit dans le cadre d'un appel de my_log ou devlog. A chaque appel
			on récupère l'instance statique du logger en appellant build_logger avec 				
			NULL et NULL. Si c'est la première fois et que build_logger n'a pas été 				
			appellé, build_logger malloc la variable logger et set les paramètres par 				
			défaut.

		Le cas de figure ne s'est pas posé, mais il est possible de modifier le
		 fonctionnement du logger en cours de runtime en réalisant 
		un nouvel appel à build_logger et en lui passant les paramètres 
		opt et parameters.
	
	Fonctions privées:

	t_logger	*get_logger();

		récupère l'instance static de logger. build_logger est appellé avec les paramètres
		NULL et NULL.


	int		set_level(t_logger **logger, char *level);

		Compare level avec les string "debug", "info", "warning" et "error".
		Set logger->lvl et logger->level en fonction.
		Retourne 1 si erreur, 0 si ok.

	
	int		set_parameters( t_logger **logger, t_chain *parameters, char *opt);

		C'est dans cette fonction qu'est gérer la définition du logger en fonction d'un appel
		à -v avec ou sans paramètre. C'est aussi là qu'est set le file_path si l'option -log est 		
		Appellée.

	Get_default_logger et build_default_logger fonctionnent comme leurs homologues pas default... servent pour devlog().
