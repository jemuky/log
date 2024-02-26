@echo off

git config user.name jymk
git config user.email jymkyu@outlook.com
git add .
git commit

if %commit_err% neq 0 (
    @echo error: nothing to commit or commit failed
) else (
    git push
)
