FROM debian:stretch

RUN apt-get update
RUN apt-get install -y \
    gcc \
    curl \
    python \
    python-pip \
    libboost-all-dev \
    unixodbc-dev \
    wget

# You can also use 8.x
RUN curl -sL https://deb.nodesource.com/setup_10.x | bash - \ 
    && apt-get install -y nodejs
RUN npm i npm@latest -g
RUN python -m pip install --upgrade pip
RUN yes | pip install cmake 

RUN mkdir -p /cacheodbc
WORKDIR /cacheodbc

COPY ./scripts ./scripts
RUN chmod +x ./scripts/odbc-install.sh && ./scripts/odbc-install.sh

COPY . .
RUN cp -rf "config/." /etc/

RUN npm install --unsafe-perm

ENTRYPOINT [ "bash" ]