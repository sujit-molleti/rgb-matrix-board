# Agent Memory

This file tracks the working agreement, review feedback, and recurring project
context for Codex-assisted maintenance of `rgb-matrix-board`.

## Working Agreement

- Work starts from a user prompt and is delivered through a GitHub pull request.
- Each task uses a short-lived branch from the current default branch.
- Pull requests should include a concise summary, the checks that were run, and
  any known risks or blockers.
- Review feedback that should affect future work belongs in this file.
- Keep implementation changes scoped to the task unless a broader edit is needed
  to keep the project coherent.

## Repository Context

- Project type: PlatformIO C++ firmware for an ESP32 RGB LED matrix.
- Main build configuration: `platformio.ini`.
- Tests: PlatformIO Unity suites under `test/`.
- Baseline local checks for PRs:
  - `pio run`
  - `pio test --without-uploading --without-testing`

## PR Workflow

1. Sync the default branch.
2. Create a branch named `codex/<short-task-name>`.
3. Implement the requested change.
4. Run the full available local check suite.
5. Commit the code changes and any relevant memory updates.
6. Push the branch.
7. Open a GitHub PR with `gh pr create`, including the summary and check
   results.

## Current Setup State

- Repository cloned locally at `/root/rgb-matrix-board`.
- SSH clone/push was attempted first, but this environment did not have a usable
  GitHub SSH key for the repository.
- HTTPS clone succeeded.
- GitHub CLI (`gh`) and PlatformIO were installed during setup.
- `gh` is authenticated and should be used to create pull requests after coding
  task branches are pushed.
- HTTPS push to GitHub is configured.

## Feedback Log

- Initial preference: use GitHub PRs for review.
- Initial preference: use `gh pr create` once GitHub CLI authentication is
  available.
- Initial preference: commit this memory file with each PR so process memory is
  visible in review.
- Initial preference: run the full available test suite before opening PRs.
- Follow-up preference: after pushing coding task branches, create the PR with
  `gh pr create` instead of stopping at the GitHub compare URL.
