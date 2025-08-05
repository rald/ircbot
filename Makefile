bot: bot.c util.c util.h knowledge.c knowledge.h
	gcc bot.c dyad.c util.c knowledge.c -o bot -g

clean:
	rm bot
