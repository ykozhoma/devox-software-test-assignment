FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# Install asio for networking.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends libasio-dev

RUN git config \
--global --add safe.directory /workspaces/devox-software-test-assignment
