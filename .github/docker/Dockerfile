FROM ubuntu:20.04

RUN apt update && apt upgrade -y \
    && DEBIAN_FRONTEND="noninteractive" apt install -y --no-install-recommends \
        git make cmake gcc g++ python libpthread-stubs0-dev \
    && apt install -y wget

RUN wget -qc https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2 -O gcc-arm.tar.bz2 \
    && tar xjf gcc-arm.tar.bz2 -C /usr/local \
    && rm -r gcc-arm.tar.bz2 \
    && mv /usr/local/gcc-arm-* /usr/local/gcc-arm

ENV PATH="/usr/local/gcc-arm/bin:${PATH}"

RUN useradd -ms /bin/bash build \
    && mkdir /home/build/project
USER build
WORKDIR /home/build/project/build

VOLUME  /home/build/project

CMD ["/usr/bin/cmake"]
