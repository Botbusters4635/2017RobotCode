OSFLAG :=
HOST_ROBORIO :=  lvuser@roboRIO-4635-frc.
COMMAND := ./gradlew

UPLOAD_SETTINGS := scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ./settings*.ini $(HOST_ROBORIO):/home/lvuser/.
RETRIEVE_SETTINGS:= scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null $(HOST_ROBORIO):/home/lvuser/settings*.ini .

UPLOAD_PATHS := scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ./paths/*.path $(HOST_ROBORIO):/home/lvuser/paths/
RETRIEVE_PATHS := scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null $(HOST_ROBORIO):/home/lvuser/paths/*.path paths/
CLEAN_PATHS := ssh $(HOST_ROBORIO) rm -f /home/lvuser/paths/*.path 

RETRIEVE_LOGS := scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null $(HOST_ROBORIO):/home/lvuser/logs/*.csv logs/
CLEAN_LOGS_RIO := ssh $(HOST_ROBORIO) rm -f /home/lvuser/logs/*.csv

ifeq ($(OS),Windows_NT)
	OSFLAG = -D WIN32
	COMMAND = .\gradlew.bat
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG = AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		OSFLAG = IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG = LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG = OSX
	endif
		UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		OSFLAG = AMD64
	endif
		ifneq ($(filter %86,$(UNAME_P)),)
	OSFLAG = IA32
		endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		OSFLAG = ARM
	endif
endif

all:
	@echo Building source code...
	@$(COMMAND) build

build_deploy: all
	@echo Building and deploying to robot...
	@$(UPLOAD_SETTINGS)
	@$(UPLOAD_PATHS)
	@$(COMMAND) build deploy --offline
clean:
	@echo Cleaning build folder...
	@$(COMMAND) clean

deploy:
	@echo Deploying to robot...
	@$(UPLOAD_SETTINGS)
	@$(UPLOAD_PATHS)
	@$(COMMAND) deploy --offline

put_settings:
	@echo Updating settings.ini on robot...
	@$(UPLOAD_SETTINGS)

get_settings:
	@echo Retrieving settings.ini from robot...
	@$(RETRIEVE_SETTINGS)

get_logs:
	@echo Retrieving logs from robot...
	@$(RETRIEVE_LOGS)

clean_logs:
	@echo Deleting logs on robot...
	@$(CLEAN_LOGS_RIO)

put_paths:
	@echo Updating paths on robot...
	@$(UPLOAD_PATHS)

get_paths:
	@echo Retrieving paths from robot...
	@$(RETRIEVE_PATHS)
