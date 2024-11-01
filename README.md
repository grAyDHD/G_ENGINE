G_ENGINE Git Workflow Guide

This guide outlines the process for contributing to the G_ENGINE repository. The main workflow involves making changes in either the library or engine branches. Each contribution begins with a feature branch under one of these primary branches, and once changes are tested, the feature branches are merged back into library or engine. Finally, the updated library and engine branches are merged into main. Workflow Summary

Start with an Up-to-Date library or engine Branch
Create a Feature Branch Under library or engine
Make Changes, Commit, and Push the Feature Branch
Merge the Feature Branch into library or engine
Merge library and engine into main

Step-by-Step Guide

    Start with an Up-to-Date library or engine Branch

Begin by checking out the relevant primary branch (library or engine) and making sure it is up-to-date with the latest changes from the remote repository.

bash
Ensure you're on the latest main branch

git checkout main git pull origin main
Check out and pull updates for the 'library' branch

git checkout library git pull origin library
Or, check out and pull updates for the 'engine' branch

git checkout engine git pull origin engine

    Create a Feature Branch for Your Work

Create a new feature branch under either library or engine, depending on where your changes are focused.

bash
From library branch

git checkout library git checkout -b library-feature-branch
Or from engine branch

git checkout engine git checkout -b engine-feature-branch

    Make Changes, Commit, and Push the Feature Branch

    Make your changes in the relevant directory (Library for library changes, NG for engine changes). Stage the files and commit your changes with a descriptive message.

bash
Stage your changes

git add # Add a specific file git add . # Or add all changes
Commit with a message

git commit -m "Describe the feature or fix"

Push the feature branch to the remote repository.

bash

git push -u origin library-feature-branch # or engine-feature-branch

    Merge the Feature Branch into library or engine

    After testing and reviewing, merge your feature branch back into the main library or engine branch.

bash
For library branch

git checkout library git merge library-feature-branch
For engine branch

git checkout engine git merge engine-feature-branch

Push the updated library or engine branch to the remote repository.

bash

git push origin library # Or git push origin engine

Optionally, delete the feature branch locally and remotely after merging.

bash

git branch -d library-feature-branch git push origin --delete library-feature-branch # or engine-feature-branch

    Merge library and engine into main

Once library and engine have stable updates, merge each of them into main.

bash
Merge library into main

git checkout main git merge library git push origin main
Merge engine into main

git merge engine git push origin main

Explanation of Commands

Here’s what each Git command does in this workflow:

git checkout <branch-name>: Switches to the specified branch.
git pull origin <branch-name>: Updates the local branch with changes from the remote branch.
git checkout -b <new-branch-name>: Creates a new branch and switches to it.
git add <file-path> or git add .: Stages files to be included in the next commit.
git commit -m "message": Commits staged changes to the branch with a descriptive message.
git push -u origin <branch-name>: Pushes the branch to the remote repository and sets it to track the remote branch.
git merge <branch-name>: Merges changes from the specified branch into the current branch.
git branch -d <branch-name>: Deletes the local branch after it has been merged.
git push origin --delete <branch-name>: Deletes the specified branch from the remote repository.

This workflow keeps main stable by allowing all feature work to be contained in separate branches under library or engine, promoting clean, organized development.
