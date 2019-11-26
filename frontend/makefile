default: run

install-angular:
	npm install -g angular-cli

install-npm-packages:
	cd viewer && npm install
	cd server && npm install

move-viewer:
	cd viewer && cp -p -R dist ../server/dist

build: clean
	cd viewer && ng build && cp -p -R dist ../server/dist

.PHONY: build

clean:
	-rm -rf server/dist
.PHONY: clean

run:
	cd server && node server.js
.PHONY: run

build-and-run: build run

.PHONY: build-and-run
