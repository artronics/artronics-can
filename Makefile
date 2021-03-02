docker-build:
	docker build -t artronics/artronics-firmware:latest -f .github/build/Dockerfile .github/build

build-firmware:
	docker run -it --rm -v $(shell pwd):/home/build/project artronics/artronics-firmware cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake --build . --target artronics.out ..

firmware: build-firmware
	docker run -it --rm -v $(shell pwd):/home/build/project artronics/artronics-firmware make artronics.out

build-tests:
	docker run -it --rm -v $(shell pwd):/home/build/project artronics/artronics-firmware cmake -DTEST=ON ..

tests: build-tests
	docker run -it --rm -v $(shell pwd):/home/build/project artronics/artronics-firmware make all_tests
	docker run -it --rm -v $(shell pwd):/home/build/project artronics/artronics-firmware ctest

clean:
	rm -rf build/*
