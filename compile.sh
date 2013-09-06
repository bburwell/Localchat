#!/bin/sh

gcc -pthread -o localchat \
	localchat.c \
	chat_acceptor.c \
	chat_request.c \
	check_user_name.c \
	clean_table.c \
	loading.c \
	online.c \
	own_ip.c \
	parse.c \
	receive.c \
	status.c \
	user_command.c
