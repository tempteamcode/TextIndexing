# TextIndexing

## Utilisation

### Programme TextIndexer en ligne de commande

#### Compilation

Le dossier "backend" contient les sources du programme principal, écrit en C++.

Un makefile est fourni pour générer l'exécutable TextIndexer (avec la commande `make build`).

#### Indexation de texte et génération de l'InvertedFile

Pour générer l'InvertedFile sur un corpus de texte, il faut s'assurer qu'un fichier texte
"inputs.txt" soit présent dans le même dossier que le programme TextIndexer, et contienne
les chemins (relatifs) des fichiers textes à indexer (un chemin par ligne).

Le contenu du fichier "inputs.txt" pourra être, par exemple :
```
latimes\la010189
latimes\la010289
latimes\la010389
```

Exécuter TextIndexer avec la commande `./TextIndexer index` déclenchera la génération de l'InvertedFile.
Celui-ci sera construit dans le dossier contenant le programme TextIndexer, et sera appelé "InvertedFile.bin".

#### Requêtes

Après avoir généré l'InvertedFile, il est possible d'exécuter des requêtes avec la commande `./TextIndexer query <mots>`,
où "<mots>" est l'ensemble des mots recherchés, séparés par des espaces.
Le programme donnera alors les 10 résultats les plus pertinents, pour différentes méthodes de recherche.

Pour avoir plus d'informations sur les documents résultats d'une requête, il est possible d'exécuter la commande `./TextIndexer document_json <ID>`,
où "<ID>" est le numéro d'un document tel qu'affiché dans les résultats de recherche.
Le programme retournera alors la date, le titre, et le contenu de ce document, le tout dans un format JSON.

### Viewer en interface graphique

#### Prérequis

- Installer node.js ainsi que npm (installé avec node)
- Installer le package angular-cli globalement
  - Dans le dossier viewer, executer `make install-angular`
- Install les packages locaux
  - Dans le dossier viewer, executer `make install-npm-packages`

#### Process

La commande `make build-and-run` va compiler le viewer et le copier dans le serveur puis lancer le serveur sur un port indiqué.

Plus tard pour relancer le serveur sans recompiler le viewer `make run`

#### Ouvrir le viewer

Rendez vous sur votre navigateur préféré et tapez : 
```
localhost:<le_port_donné_par_le_serveur>
```
Normalement, le port est le **1337**.
