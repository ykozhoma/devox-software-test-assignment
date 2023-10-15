# Start Here

1. Use this template repository to create a private fork (click the green `Use this template` button and not the `Fork` button).
1. Follow the instructions in `INSTRUCTIONS.md`.
1. Give [@tes-mersive](https://github.com/tes-mersive) (`estephens@mersive.com`) collaborator access when complete.
1. Inform your Mersive contact that the assignment is complete.

# Your Documentation

**Build and Run instructions.**

You can run this project in two ways:
- _Using your local machine_
- _Using Docker and Visual Studio Code_

_On your Ubuntu/Debian local machine._

**Package requirments: compiler with C++ 17 support, cmake, make.**

1. Install libasio-dev

    _For Ubuntu:_

    ```
    sudo apt update \
    && sudo apt install -y --no-install-recommends libasio-dev

    ```
2. Run build and server binary
```
    cd devox-software-test-assignment
    chmod +x ./buid/run.sh
    cd build
    ./run.sh
```

3. Open new terminal window and try one of the curl commands from INSTRUCTIONS.md

_From VSCode:_

1. Open project folder in Visual Studio Code.

2. Make sure you have Docker installed on your local machine.

3. If you have Remote SSH and Docker extensions installed in your VS Code \
    follow VS Code's instructions and run the app inside the Docker container.

4. The image has all necessary dependencies pre-installed.

5. VS Code should build the image from ./devcontainer/Dockerfile and open project folder in terminal.

6. After Docker image build succeeds run the following commands.
```
    chmod +x ./build/run.sh
    cd build
    ./run.sh
```
7. Now you can connect to server either in new VS Code terminal Or from your local machine using `http://localhost:8000`.

**Ways to further improve**
- add JSON serialization and store requests in json format, rather then in regular structures;
- add request validation in http parser;
- add non-destructive updates;