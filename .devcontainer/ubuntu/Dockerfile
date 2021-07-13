FROM ubuntu:focal

ENV DEBIAN_FRONTEND=noninteractive

RUN sed --in-place=~ 's|archive.ubuntu.com|ftp.acc.umu.se|g' /etc/apt/sources.list

# base-tools for C and C++
RUN apt-get update \
    && apt-get install \
        --no-install-recommends \
        --assume-yes \
        --quiet \
        autoconf \
        automake \
        cmake \
        g++ \
        gcc \
        gdb \
        libtool \
        make \
        pkg-config \
    && rm -r /var/lib/apt/lists/*

# Other useful tools for C and C++
RUN apt-get update \
    && apt-get install \
        --no-install-recommends \
        --assume-yes \
        --quiet \
        clangd \
        valgrind \
    && rm -r /var/lib/apt/lists/*

# Other useful tools.
RUN apt-get update \
    && apt-get install \
        --no-install-recommends \
        --assume-yes \
        --quiet \
        ca-certificates \
        curl \
        git \
        less \
        nano \
        sudo \
        xz-utils \
    && rm -r /var/lib/apt/lists/*

# Project-specific dependencies
RUN apt-get update \
    && apt-get install \
        --no-install-recommends \
        --assume-yes \
        --quiet \
        libssl-dev \
    && rm -r /var/lib/apt/lists/*

RUN useradd \
        --uid 1000 \
        --shell /bin/bash \
        --create-home \
        build \
    && adduser build sudo \
    && echo "\n# Allow sudo without password\n%sudo ALL=(ALL) NOPASSWD:ALL\n" >> /etc/sudoers

# Don't run as root inside container,
# see https://github.com/microsoft/vscode-remote-release/issues/22
ENV HOME /home/build
ENV THIRD_PARTY_ROOT /home/build/third_party
ENV DIST ubuntu
USER build

CMD ["/bin/bash", "-l"]
