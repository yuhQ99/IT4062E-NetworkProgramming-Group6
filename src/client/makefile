TARGET=client
objs=client.o callback.o WindowMain.o account.o room.o windowchat.o
CFLAGS= -g -Wall `pkg-config --cflags gtk+-3.0` -Wl,--export-dynamic
LDFLAGS= `pkg-config --libs gtk+-3.0` -lpthread -Wl,--export-dynamic

$(TARGET):$(objs)
	gcc $^ -o $@ $(LDFLAGS)
	rm -rf $(objs)
%.o:%.c
	gcc -c $< $(CFLAGS)

.PHONY:clean
clean:
	rm -rf $(TARGET) $(objs)