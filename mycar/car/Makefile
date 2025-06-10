# name of your application
APPLICATION = car

# If no BOARD is found in the environment, use this default:
BOARD ?= esp32-wroom-32

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1
USEMODULE += auto_init
USEMODULE += xtimer
USEMODULE += periph_gpio

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Use a peripheral timer for the delay, if available

include $(RIOTBASE)/Makefile.include
