# nss http

nss-http is a plugin to query unix accounts over http(s). It is intended to be
used for storing unix accounts globally in one common database, instead of
manually keeping them synchronized locally on each computers.

See server implementation @ <https://github.com/1nfiniteloop/unix-accounts>.

## Install

Get debian package from latest release below, or build local debian package, see
[Build](Build).

    curl \
      --silent \
      --remote-name \
      https://github.com/1nfiniteloop/nss-http/releases/download/v1.0.0/nss-http-1.0.0-Linux.deb

Install package

    sudo dpkg -i nss-http-1.0.0-Linux.deb

## Configure

Add http into `/etc/nsswitch.conf`. Make sure http is placed at the end, local
databases shall be queried first.

    passwd:         files http
    group:          files http
    shadow:         files http

Configuration files for nss-http are located in `/etc/nss-http`.

* `shadow.auth-token.base64`: Add your token here generated from
  `unix-accounts-server --generate-token` __with an additional new line__ after
  the token. Permissions on this file controls who can query the password
  database, they shall be equal to permissions on `/etc/shadow`.
* `configuration.yaml`: contains configuration for server url, endpoints and tls
  client certificates, adjust accordingly.

Errors are written to syslog tagged as "nss-http".

## Additional configurations

It's also convenient to get a home folder created for new users, configure
`/etc/pam.d/login` according to below. See more @
<http://linux-pam.org/Linux-PAM-html/sag-pam_mkhomedir.html>

    ...
    # Create home for new users
    session    required    pam_mkhomedir.so skel=/etc/skel/ umask=0022

    # Standard Un*x authentication.
    @include common-auth

It is recommended to use nss-http plugin with name service cache daemon
`nscd`. Install with:

    sudo apt-get install nscd

Configure `/etc/nscd.conf` and keep only records for "group" and "passwd".

## Develop

### Overview

The build-environment is based on a portable docker-container including all
basic tools needed, see more in `.devcontainer/ubuntu/Dockerfile`.  No further
package installations is required, the only prerequisite is that Docker is
installed.

Visual Studio Code (VS Code) together with the plugin
_"ms-vscode-remote.remote-containers"_ makes it very convenient to have the
development environment inside a container, see more @
<https://code.visualstudio.com/docs/remote/containers>. To setup the development
environment simply open this project in VS Code. A notification with an option
to open this project inside the container will appear. Recommended VS Code
extensions will also be installed and set-up accordingly, see more in
`.devcontainer/devcontainer.json`.

### Install dependencies

__Note:__ This shall be run from inside the container. The dependencies is
cached on a docker volume under `~/third_party`.

    third-party/gtest \
      && third-party/boost \
      && third-party/cpr \
      && third-party/yaml-cpp

### Build

1. Create build tree with `mkdir --parents build/{Debug,Release}`

2. Build _Debug_ and run tests:

        cd build/Debug \
        && cmake \
            -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
            -D VERSION=1.0.0 \
            -D BUILD=Debug \
            ../../ \
        && make test_all

3. Build _Release_ and make package:

        cd build/Release \
          && cmake \
             -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
             -D VERSION=1.0.0 \
             -D BUILD=Release \
             -D UNITTEST=OFF \
             ../../ \
          && make package

### Test

Login with nss-http can be tested from a container with:

    docker exec -it -u root nss-http.vscode /bin/login foo

nscd can be started in a container with

    sudo /etc/init.d/nscd start

## Reference

* glibc nss source: <https://sourceware.org/git/?p=glibc.git;a=tree>
* glibc unofficial github mirror:
  <https://github.com/bminor/glibc/tree/master/nss>
* Name service switch:
  <https://www.gnu.org/software/libc/manual/html_node/Name-Service-Switch.html>
* libc user and groups:
  <https://www.gnu.org/software/libc/manual/html_node/Users-and-Groups.html>
* Another nss-http implementation served as inspiration:
  <https://github.com/gmjosack/nss_http>
