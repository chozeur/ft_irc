#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

int main(int argc, char *argv[])
{
	// Vérification des arguments de la ligne de commande
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <IP address> <port>" << std::endl;
		return 1;
	}

	/*
	MODE D'EMPLOI - Création d'une socket pour accepter les connexions entrantes

	Cette partie du code crée une nouvelle socket pour accepter les connexions entrantes. La fonction socket() prend trois arguments :
	Le premier argument :
		le domaine de la socket. Dans ce cas-ci, nous utilisons le domaine AF_INET qui correspond au protocole IPv4.
	Le deuxième argument :
		le type de la socket. Nous utilisons ici SOCK_STREAM pour une socket de type stream, qui fournit une connexion fiable de bout en bout.
	Le troisième argument :
		le protocole à utiliser pour la socket. 
		Nous utilisons 0 pour indiquer que le système d'exploitation doit choisir le protocole approprié pour nous.
	La fonction socket() renvoie un descripteur de fichier qui identifie la nouvelle socket. Si la fonction renvoie -1, cela signifie qu'une erreur s'est produite lors de la création de la socket. 
	Dans ce cas, le code affiche un message d'erreur et retourne une valeur non nulle pour indiquer qu'il y a eu une erreur.
	*/

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		return 1;
	}

	/*
	MODE D'EMPLOI - Configuration de l'adresse et du port du serveur

	Cette partie du code configure l'adresse et le port du serveur.

	La première ligne de code :
		récupère l'adresse IP fournie en ligne de commande et la stocke dans la variable ip_address.
	La deuxième ligne de code :
		récupère le numéro de port fourni en ligne de commande et le stocke dans la variable port.

	La structure sockaddr_in est utilisée pour représenter l'adresse et le port du serveur. Les champs de cette structure sont initialisés de la manière suivante :

	Le champ sin_family est initialisé à AF_INET:
		Cela spécifie que nous utilisons le protocole IPv4.
	Le champ sin_addr.s_addr est initialisé avec l'adresse IP du serveur
		La fonction inet_addr() convertit une adresse IP au format texte en un format binaire approprié pour être utilisé par la socket.
	Le champ sin_port est initialisé avec le numéro de port du serveur
		La fonction htons() convertit le numéro de port dans l'ordre d'octets réseau (big-endian) en l'ordre d'octets hôte (little-endian), qui est le format utilisé par l'ordinateur sur lequel s'exécute le programme.
	Une fois que la structure sockaddr_in est initialisée avec l'adresse et le port du serveur, elle peut être utilisée pour lier la socket à l'adresse et au port en appelant la fonction bind().
	*/

	std::string 		ip_address = argv[1];
	int 				port = std::stoi(argv[2]);
	struct sockaddr_in 	server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	server_address.sin_port = htons(port);

	/*
	MODE D'EMPLOI - Lier la socket à l'adresse et au port du serveur

	Cette partie du code lie la socket à l'adresse et au port spécifiés par la structure server_address.

	La fonction bind() prend trois arguments :

	Le descripteur de la socket que nous avons créé précédemment et stocké dans la variable sockfd.
	Un pointeur vers une structure sockaddr qui contient l'adresse et le port du serveur. Cette structure doit être convertie en un pointeur de type sockaddr * pour pouvoir être utilisée comme argument de la fonction bind(). C'est ce que fait l'expression (struct sockaddr *)&server_address.
	La taille de la structure sockaddr, qui peut être obtenue en appelant la fonction sizeof().
	Si la liaison réussit, la fonction bind() renvoie 0. Si la liaison échoue, elle renvoie -1 et nous affichons un message d'erreur et terminons le programme en retournant 1.
	*/

	if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		std::cerr << "Error: bind failed" << std::endl;
		return 1;
	}

	/*
	MODE D'EMPLOI - Mettre la socket en mode d'écoute

	Cette partie du code place la socket en mode d'écoute pour les connexions entrantes.

	La fonction listen() prend deux arguments :

	Le descripteur de la socket que nous avons créé précédemment et stocké dans la variable sockfd.
	Le nombre maximum de connexions en attente que la socket peut gérer. Dans ce cas, nous avons spécifié 10, ce qui signifie que la socket peut gérer jusqu'à 10 connexions en attente.
	Si la fonction listen() réussit, elle renvoie 0. Si la fonction échoue, elle renvoie -1 et nous affichons un message d'erreur et terminons le programme en retournant 1.
	*/

	if (listen(sockfd, 10) == -1)
	{
		std::cerr << "Error: listen failed" << std::endl;
		return 1;
	}

	std::cout << "Server listening on " << ip_address << ":" << port << std::endl;

	/*
	MODE D'EMPLOI - Définir un tableau de structures pollfd pour surveiller les sockets

	Cette partie du code permet de définir un tableau de structures pollfd pour surveiller les sockets. 
	La structure pollfd est utilisée pour spécifier les événements que nous souhaitons surveiller sur un descripteur de fichier, tels que la réception de données ou la fermeture de la connexion.

	La variable MAX_CLIENTS indique le nombre maximum de clients que notre serveur peut gérer. 
	Nous avons défini un tableau de structures pollfd pour surveiller les sockets de chaque client, ainsi que la socket d'écoute.

	Le tableau fds est initialisé en ajoutant la socket d'écoute dans le premier élément de tableau, et en initialisant tous les autres éléments de tableau à -1 pour indiquer que les sockets clients sont vides.

	Chaque élément de tableau pollfd contient deux membres :
		fd est le descripteur de la socket à surveiller.
		events est un masque de bits qui indique les événements à surveiller sur la socket.
	
	Dans ce cas, nous surveillons les événements de réception de données sur toutes les sockets, en définissant le membre events à POLLIN.
	Cette configuration nous permet de surveiller simultanément les connexions de plusieurs clients et de détecter les événements de réception de données sur ces connexions.
	*/

	const int MAX_CLIENTS = 10;			// Nombre maximum de clients
	struct pollfd fds[MAX_CLIENTS + 1]; // +1 pour la socket d'écoute
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;
	for (int i = 1; i <= MAX_CLIENTS; ++i)
	{
		fds[i].fd = -1; // Initialise les sockets client à -1 pour indiquer qu'elles sont vides
		fds[i].events = POLLIN;
	}

	// Boucle principale pour accepter les connexions entrantes
	while (true)
	{

		/*
		Cette partie du code utilise la fonction poll pour surveiller les changements d'état dans les sockets surveillées. 
		La fonction poll prend en entrée un tableau de structures pollfd qui contient les descripteurs de fichiers des sockets à surveiller, le nombre de sockets à surveiller et le temps d'attente maximal (en millisecondes) pour qu'une activité soit détectée.

		Dans ce cas, la fonction poll surveille les sockets dans le tableau fds qui contient la socket d'écoute et jusqu'à 10 sockets client. 
		Lorsque l'état d'une socket change (par exemple, lorsqu'un client se connecte ou envoie un message), la fonction poll renvoie le nombre de sockets prêtes pour une activité (num_ready_fds). 
		Si une erreur se produit, la valeur renvoyée est -1.

		Dans le code, si la fonction poll renvoie -1, cela indique qu'une erreur s'est produite lors de la surveillance des sockets, et la boucle principale est interrompue.
		*/
		int num_ready_fds = poll(fds, MAX_CLIENTS + 1, -1);
		if (num_ready_fds == -1)
		{
			std::cerr << "Error: poll failed" << std::endl;
			break;
		}


		/*
		Cette partie de code utilise la variable fds qui contient un tableau de structures pollfd pour surveiller les sockets et détecter les événements associés à ces dernières. 
		Dans cette partie en particulier, le code vérifie si la socket d'écoute a détecté un événement de type POLLIN, indiquant qu'il y a une connexion entrante en attente.

		Si tel est le cas, la fonction accept est appelée pour accepter la connexion entrante. 
		Si l'appel de la fonction accept renvoie -1, cela signifie qu'une erreur s'est produite et le code continue à surveiller les sockets en attente de nouveaux événements.

		Si l'appel de la fonction accept réussit, la nouvelle socket client est ajoutée au tableau fds dans la première entrée libre. Le code affiche ensuite un message indiquant qu'un nouveau client s'est connecté
		*/
		if (fds[0].revents & POLLIN)
		{
			// Accepter une connexion entrante
			struct sockaddr_in client_address;
			socklen_t client_address_length = sizeof(client_address);
			int clientfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_length);
			if (clientfd == -1)
			{
				std::cerr << "Error: accept failed" << std::endl;
				continue;
			}
			// Ajouter la socket du client à l'ensemble des sockets surveillées
			for (int i = 1; i <= MAX_CLIENTS; ++i)
			{
				if (fds[i].fd == -1)
				{
					fds[i].fd = clientfd;
					std::cout << "New client connected" << std::endl;
					break;
				}
			}
		}

		/*
		MODE D'EMPLOI - Traiter les sockets clientes

		Dans cette partie du code, la boucle parcourt le tableau de structures pollfd pour voir si des données sont disponibles sur l'un des descripteurs de fichiers surveillés. 
		Si une socket de client est prête à être lue (c'est-à-dire qu'elle a des données en attente), la fonction recv est appelée pour lire les données et les stocker dans un tampon de 1024 octets.

		Si la fonction recv renvoie -1, cela indique une erreur de lecture, et la connexion du client est fermée en appelant la fonction close. 
		Si elle renvoie 0, cela signifie que le client a fermé la connexion, et la connexion du client est également fermée en appelant la fonction close.

		Si la fonction recv renvoie une valeur positive, cela signifie que des données ont été lues avec succès, et elles sont stockées dans une chaîne de caractères message qui est affichée dans la console.
		*/
		for (int i = 1; i <= MAX_CLIENTS; ++i)
		{
			if (fds[i].fd != -1 && fds[i].revents & POLLIN)
			{
				char buffer[1024];
				int bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytes_received == -1)
				{
					std::cerr << "Error: message receiving failed" << std::endl;
					close(fds[i].fd);
					fds[i].fd = -1;
				}
				else if (bytes_received == 0)
				{
					std::cout << "Client closed the connection" << std::endl;
					close(fds[i].fd);
					fds[i].fd = -1;
				}
				else
				{
					std::string message(buffer, bytes_received);
					std::cout << message;
				}
			}
		}

		// À ce stade, vous pouvez traiter les commandes envoyées par les clients et envoyer des messages aux clients en utilisant les fonctions send et recv

	}

	// Fermer les sockets clientes
	for (int i = 1; i <= MAX_CLIENTS; ++i)
	{
		if (fds[i].fd != -1)
		{
			close(fds[i].fd);
		}
	}

	// Fermer la socket d'écoute
	close(sockfd);

	return 0;
}
