# 42sh

## Description
**42sh** est un projet visant à implémenter un interpréteur de commandes POSIX-compatible (shell). Ce projet met l'accent sur la gestion des commandes simples, des redirections, des boucles, et des fonctionnalités avancées comme les substitutions de commandes et les alias.

Ce document décrit comment configurer, utiliser et étendre **42sh**.

---

## Fonctionnalités
- Exécution de commandes simples.
- Gestion des redirections (`>`, `>>`, `<`).
- Pipelining (`|`).
- Commandes conditionnelles (`if`, `while`, `for`).
- Gestion des variables environnementales (`$PWD`, `$UID`, etc.).
- Support des commentaires (`#`).
- Builtins essentiels (`cd`, `exit`, `export`, `unset`, etc.).
- Prise en charge de fonctionnalités POSIX standards.

---

## Structure du Projet
```
42sh/
├── src/            # Code source principal
│   ├── lexer.c     # Analyse lexicale
│   ├── parser.c    # Analyse syntaxique
│   ├── executor.c  # Exécution des commandes
│   └── builtins/   # Implémentation des commandes built-in
├── tests/          # Tests fonctionnels et unitaires
├── Makefile.am     # Fichier pour le système de build Autotools
└── configure.ac    # Script de configuration
```

---

## Prérequis
- **Compilateur C** : GCC avec support pour `-std=c99`.
- **Autotools** : `autoreconf`, `automake`, `autoconf`.
- **Librairies** : POSIX standard.

---

## Installation
1. Clonez le dépôt :
   ```bash
   git clone <URL_du_dépôt>
   cd 42sh
   ```
2. Configurez le projet :
   ```bash
   autoreconf --install
   ./configure
   ```
3. Compilez :
   ```bash
   make
   ```
4. Installez :
   ```bash
   make install
   ```

---

## Utilisation
### Modes d'entrée :
1. **Chaîne en ligne de commande** :
   ```bash
   ./42sh -c "echo Hello World"
   ```
2. **Depuis un fichier** :
   ```bash
   ./42sh script.sh
   ```
3. **Standard Input (stdin)** :
   ```bash
   ./42sh
   ```

---

## Tests
Lancez la suite de tests avec :
```bash
make check
```

### Structure des Tests
Les tests sont organisés par catégorie dans le répertoire `tests/`. Chaque catégorie contient des fichiers de tests dédiés à une fonctionnalité spécifique, comme les pipelines, les commandes simples, etc.

---

## Consignes Importantes
- **Libération des ressources** : Toute allocation dynamique doit être libérée.
- **Compatibilité POSIX** : Respect strict des spécifications POSIX pour les commandes et le comportement.
- **Fonctions interdites** : `system`, `popen`, `syscall`, etc.
- **Style de code** : Utilisation stricte des règles de codage EPITA.

---

## Astuces de Développement
1. **Debugging** :
   - Utilisez la fonctionnalité de pretty-print pour afficher l'AST.
   - Visualisez avec des outils comme Graphviz.
2. **Tests robustes** :
   - Implémentez une suite de tests couvrant chaque fonctionnalité.
   - Comparez les résultats avec `bash --posix`.
3. **Outils recommandés** :
   - `valgrind` pour la détection de fuites mémoire.
   - `gdb` pour le débogage.

---

## Contributeurs
- **Équipe EPITA Assistants**
- Pour toute question, contactez : [assistants@tickets.assistants.epita.fr](mailto:assistants@tickets.assistants.epita.fr)

---

## Références
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [Documentation Bash](https://www.gnu.org/software/bash/manual/)
- [Guide Autotools](https://www.lrde.epita.fr/~adl/dl/autotools-handout-4.pdf)
