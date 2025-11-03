# build stage
FROM node:22-alpine AS build
WORKDIR /app

# lock 없이 package.json만 복사
COPY package.json ./
RUN npm install        # ← ci 대신 install (devDeps 포함)

# 나머지 소스 복사 후 빌드
COPY . .
ARG VITE_API_URL
ENV VITE_API_URL=$VITE_API_URL
RUN npm run build

# serve stage
FROM nginx:alpine
RUN apk add --no-cache netcat-openbsd
WORKDIR /usr/share/nginx/html
COPY --from=build /app/dist ./
COPY wait-for-it.sh /wait-for-it.sh
RUN chmod +x /wait-for-it.sh
EXPOSE 80
CMD ["/wait-for-it.sh", "backend", "8080", "--", "nginx", "-g", "daemon off;"]
