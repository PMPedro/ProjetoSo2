# Alvo padrão
all: manager feed

# Compilação de feed.o
feed.o: feed.c
	gcc -c feed.c -o feed.o

# Compilação de manager.o
manager.o: manager.c
	gcc -c manager.c -o manager.o

# Geração do binário manager
manager: manager.o
	gcc manager.o -o manager

# Geração do binário feed
feed: feed.o
	gcc feed.o -o feed

# Limpeza de arquivos intermediários e binários
clean:
	@find . -name 'pipe[0-9][0-9][0-9][0-9]' -exec unlink {} \; || echo "nenhum pipe encontrado."
	unlink mainpipe
	rm -f *.o manager feed mainpipe pipe*
