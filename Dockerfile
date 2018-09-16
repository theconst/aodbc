# TODO(kko): USE cmake build tool
FROM debian:stretch

RUN apt-get update
RUN apt-get install -y \
    gcc \
    curl \
    python \
    python-pip \
    unixodbc-dev \
    wget
RUN curl -sL https://deb.nodesource.com/setup_8.x | bash - \ 
    && apt-get install -y nodejs
RUN npm i npm@latest -g
RUN python -m pip install --upgrade pip
RUN yes | pip install cmake 

RUN mkdir -p /nc
WORKDIR /nc

COPY ./scripts ./scripts
RUN chmod +x ./scripts/boost-install.sh && ./scripts/boost-install.sh
RUN chmod +x ./scripts/odbc-install.sh && ./scripts/odbc-install.sh

COPY . .
RUN chmod +x ./scripts/nanodbc-build.sh && ./scripts/nanodbc-build.sh
RUN cp -rf "config/." /etc/

RUN npm install --unsafe-perm

ENTRYPOINT [ "bash" ]