# Hiking Tour Assistant

## Description

Hiking Tour Assistant is a software product that allows users to record their hiking trips on TTGO smartwartch and observe sessions on Raspberry Pi. 

## Software Modules

HTA_RPIA: Raspberry Pi application for the Hiking Tour Assistant  
HTA_SWA: TTGO Smartwatch application for the Hiking Tour Assistant  

Module specific README.md files are located in the corresponding directories.

## Development
 
Branching strategy:
- main: stable version
- dev: integration branch
- feature branches: feature development

When working on a feature of the project, branch from dev to a new feature branch and open a PR back to dev when ready. When tested on hw will merge to stable main.

```mermaid
gitGraph
    commit
    commit
    branch dev
    commit
    branch feature
    commit
    commit
    checkout dev
    merge feature
    commit
    branch feature2
    commit
    commit
    checkout dev
    merge feature2
    checkout main
    merge dev
```
