# SPI Master/Slave comm for DK51-Arduino UNO

[![Build Status](https://travis-ci.org/bertrandmartel/spi-master-slave-dk51.svg?branch=master)](https://travis-ci.org/bertrandmartel/spi-master-slave-dk51)

SPI Communication between Nordic Semiconductor DK51 board (Master) & Arduino UNO (Slave)

DK51 board will send periodically a set of command with parameters and data to Arduino SPI Slave. The latter will send back ACK or NAK response to SPI Master.

This project is based on `spi_master_with_spi_slave` example by Nordic Semiconductor that can be found in nrf51 SDK

## Pin configuration

| Description     | Arduino UNO | NRF51 DK |
|------|-------------|----------|
| -    |   GND       |  GND     | 
| SS   |   10        |  P0.02   |
| MOSI |   11        |  P0.01   |
| MISO |   12        |  P0.03   |
| SCK  |   13        |  P0.04   |

## Setup/Installation

* follow SDK/Toolchain Installation steps section of <a href="https://gist.github.com/bertrandmartel/a38315c5fe79ec5c8c6a9ed90b8df260#installation-steps">this tutorial</a>

* specify NRF51 SDK directory with :

```
export NRF51_SDK_DIR=/path/to/sdk
```

## Build

```
cd pca10028
make
```

## Upload

```
//erase firmware
nrfjprog --family  nRF51 -e

//upload firmware
nrfjprog --family  nRF51 --program _build/nrf51422_xxac.hex

//start firmware
nrfjprog --family  nRF51 -r
```

To debug your code : <a href="https://gist.github.com/bertrandmartel/a38315c5fe79ec5c8c6a9ed90b8df260#debug-your-code">check this link</a>

## License

The MIT License (MIT) Copyright (c) 2016 Bertrand Martel
