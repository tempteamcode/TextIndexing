# TextIndexing - Programme TextIndexer en ligne de commande (backend)

## Description

Le programme "TextIndexer" permet de générer un InvertedFile et d'exécuter des requêtes en ligne de commande.

Son code source se trouve dans le dossier "src", et est organisé en plusieurs parties, dans leur ordre logique :
- `DocumentExtractor` pour transformer un document au format XML en arbre avec balises et données
- `DocumentParser` pour parser les documents (au sens "article de journal") à partir d'un arbre
- `Tokenizer` pour extraire les mots contenus dans un texte
- `Stemmer` pour extraire la racine des mot (l'algorithme et son implémentation sont de Porter)
- `Stopwords` contient une liste des mots-clé qui sont ignorés
- `InvertedFile` pour la gestion des InvertedFile (ajout de données, export dans un fichier, import depuis un fichier)
- `files` pour l'écriture et la lecture de données dans des fichiers en binaire (utilisé pour exporter/importer l'InvertedFile)
- `search`, `FA` et `TA` contiennent des algorithmes de recherche (à partir d'un fragment d'InvertedFile et de mots-clé d'une requête)

Chaque fichier `.h` contient au moins une brève documentation de toutes les fonctions qui y sont déclarées.

## Utilisation

### Compilation

Un makefile est fourni pour générer l'exécutable TextIndexer (avec la commande `make build`).

### Indexation de texte et génération de l'InvertedFile

Pour générer l'InvertedFile sur un corpus de texte, il faut s'assurer qu'un fichier texte
"inputs.txt" soit présent dans le dossier "ressources" à côté du programme TextIndexer,
et contienne les chemins (relatifs) des fichiers textes à indexer (un chemin par ligne).

Le contenu du fichier "inputs.txt" pourra être, si le dossier "ressources" contient le dossier "latimes" :
```
latimes\la010189
latimes\la010289
latimes\la010389
```

Exécuter TextIndexer avec la commande `./TextIndexer index` déclenchera la génération de l'InvertedFile.
Celui-ci sera construit dans le dossier contenant le programme TextIndexer, et sera appelé "InvertedFile.bin".

Lors de la génération de l'InvertedFile, les erreurs de syntaxe (ou de hiérarchie de balises) des fichiers traités seront affichées.

ATTENTION : Une fois l'InvertedFile généré, il est important de ne pas modifier ni supprimer le fichier "inputs.txt".
Le programme "TextIndexer" en a effectivement besoin pour retrouver les documents (titre, contenu...) des résultats des recherches.

### Requêtes

Après avoir généré l'InvertedFile, il est possible d'exécuter des requêtes avec la commande `./TextIndexer query <mots>;`,
où "&lt;mots&gt;" est l'ensemble des mots recherchés, séparés par des espaces.
Le programme donnera alors les 10 résultats les plus pertinents, pour différentes méthodes de recherche.
Le temps d'exécution de chaque méthode de recherche est donné.

Pour avoir plus d'informations sur les documents résultats d'une requête, il est possible d'exécuter la commande `./TextIndexer document <ID>;`,
où "&lt;ID&gt;" est le numéro d'un document tel qu'affiché dans les résultats de recherche.
Le programme retournera alors la date, le titre, et le contenu de ce document, le tout dans un format JSON.
