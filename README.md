# TextIndexing

## Tester avec le viewer

### Prérequis

- Installer node.js ainsi que npm (installé avec node)
- Installer le package angular-cli globalement
  - Dans le dossier viewer, executer `make install-angular`
- Install les packages locaux
  - Dans le dossier viewer, executer `make install-npm-packages`

### Process

La commande `make build-and-run` va compiler le viewer et le copier dans le serveur puis lancer le serveur sur un port indiqué.

Plus tard pour relancer le serveur sans recompiler le viewer `make run`

### Ouvrir le viewer

Rendez vous sur votre navigateur préféré et tapez : 
```
localhost:<le_port_donné_par_le_serveur>
```
Normalement, le port est le **1337**.

### Documentation
Voir [./docs/README.md](./docs/README.md) ou [./docs/build/sphinx/html/index.html](./docs/build/sphinx/html/index.html) une fois la documentation compilée.
