#!/usr/local/bin/openocd3

set TARGET_AP cm33ap
set ENABLE_ACQUIRE 1
set ACQUIRE_TIMEOUT 1000
# set SMIF_BANKS { 0 {addr 0x60000000 size 0x4000000 psize 0x0000100 esize 0x0040000} }
set SMIF_BANKS {  1 {addr 0x60000000 size 0x1000000 psize 0x0000100 esize 0x0010000} }

# set QSPI_FLASHLOADER PSE84_SMIF.FLM

source [find interface/kitprog3.cfg]
transport select swd
adapter speed 12000
source [find target/infineon/pse84xgxs2.cfg]

init
reset init
targets cat1d.cm33

# Programming CM33 Secure App in RRAM
puts "\nPlease wait: Programming the application (this may take a few minutes). Progress will be shown below.\n"
flash write_image erase ../build/app_combined.hex
# puts "\nPlease wait: Verifying the application (this may take a few minutes). Progress will be shown below.\n"
# flash verify_image..\build\app_combined.hex

sleep 1000

reset
shutdown
