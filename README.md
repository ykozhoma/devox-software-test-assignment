# Start Here

1. Use this template repository to create a private fork (click the green `Use this template` button and not the `Fork` button).
1. Follow the instructions in `INSTRUCTIONS.md`.
1. Give [@tes-mersive](https://github.com/tes-mersive) (`estephens@mersive.com`) collaborator access when complete.
1. Inform your Mersive contact that the assignment is complete.

# Your Documentation

Run and build instructions.

On your Linux machine:
1. Install libasio-dev

    For Ubuntu 20.04:
    apt-get update && export DEBIAN_FRONTEND=noninteractive \
        && apt-get -y install --no-install-recommends libasio-dev

2. Run build and server binary

    cd devox-software-test-assignment
    chmod +x ./buid/run.sh
    ./buid/run.sh

3. Open other terminal window and try one of the curl commands from INSTRUCTIONS.md

Alternatively From VScode:
1. Open folder in VSCode
2. Make sure you have Docker installed on your local machine
3. If you have Remote SSH and Docker extensions installed in your VSCode \
    follow the suggestions and run the app in docker on any machine.
4. The image has asio and all other dependencies pre-installed
5. VSCode should build the image from ./devcontainer/Dockerfile and run it in terminal
6. After build succeeds just run
    chmod +x ./build/run.sh
    ./build/run.sh
7. Now you can connect to server either in new vscode terminal Or from your local machine.