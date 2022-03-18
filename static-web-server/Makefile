IMAGE = static-web-server
CONTAINER = http-higload
PORT = 8080
NGINX = 8888

build:
	mkdir build && cd build && cmake .. && $(MAKE)

build-docker:
	docker build -f ./docker/Dockerfile --tag $(IMAGE) .

docker-run:
	docker run -p $(PORT):$(PORT) --name $(CONTAINER) -t $(IMAGE)

docker-stop:
	docker stop $(CONTAINER)

docker-rm:
	docker rm -vf $(CONTAINER) || true

build-docker-nginx:
	docker build --no-cache -t nginx -f ./docker/nginx.Dockerfile .

docker-run-nginx:
	docker run -p $(NGINX):$(NGINX) --name nginx -t nginx

docker-stop-nginx:
	docker stop nginx

docker-rm-nginx:
	docker rm -vf nginx || true

func-test:
	./httptest.py

bench-static-web-server:
	wrk -t12 -c400 -d30s 'http://127.0.0.1:$(PORT)/httptest/splash.css'

bench-nginx:
	wrk -t12 -c400 -d30s 'http://127.0.0.1:$(NGINX)/httptest/splash.css'