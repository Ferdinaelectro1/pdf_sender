# Mini Serveur de Transfert de Fichiers PDF

Ce projet met en place un serveur utilisant **Boost.Asio** permettant de transférer des fichiers PDF entre clients.

---

## Cloner le projet

```bash
git clone https://github.com/Ferdinaelectro1/pdf_sender.git
cd pdf_sender
```

---

## Compilation

### Serveur

```bash
mkdir build
cd build
cmake ..
make
```

### Client

Ouvrir un autre terminal et compiler le client :

```bash
cd client
g++ client.cpp -o client -lboost_system -lpthread
```

> ⚠️ Assurez-vous que Boost est installé sur votre machine.

---

## Exécution et test

### 1. Démarrer le serveur

Dans le dossier `build` :

```bash
./pdf_sender
```

### 2. Tester un client récepteur (ex : ID 125)

Dans un terminal séparé :

```bash
./client 125
```

Le client attendra les fichiers envoyés par d'autres clients.

### 3. Tester un client émetteur (ex : ID 342)

Dans un autre terminal, envoyer un PDF nommé `file.pdf` au client 125 :

```bash
./client 342 125 file.pdf
```

Le serveur se chargera de transférer le fichier au destinataire.

---

## Notes

* Le serveur écoute sur le port **12345**.
* Chaque client doit s'identifier par un **ID**.
* Le protocole garantit l'intégrité des fichiers PDF envoyés, même si ceux-ci contiennent des caractères spéciaux ou des `\0`.
* Les fichiers PDF reçus sont sauvegardés côté client sous le nom `received.pdf`.
