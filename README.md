# [Set (card game)](https://en.wikipedia.org/wiki/Set_(card_game))

> Set (stylized as SET) is a real-time card game designed by Marsha Falco in 1974 and published by Set Enterprises in 1991. The deck consists of 81 unique cards that vary in four features across three possibilities for each kind of feature: number of shapes (one, two, or three), shape (diamond, squiggle, oval), shading (solid, striped, or open), and color (red, green, or purple). Each possible combination of features (e.g. a card with three striped green diamonds) appears as a card precisely once in the deck.
>
> In the game, certain combinations of three cards are said to make up a set. For each one of the four categories of features — color, number, shape, and shading — the three cards must display that feature as either a) all the same, or b) all different. Put another way: For each feature the three cards must avoid having two cards showing one version of the feature and the remaining card showing a different version.
>
> For example, 3 solid red diamonds, 2 solid green squiggles, and 1 solid purple oval form a set, because the shadings of the three cards are all the same, while the numbers, the colors, and the shapes among the three cards are all different.
>
> For any "set", the number of features that are all the same and the number of features that are all different may break down as 0 the same + 4 different; or 1 the same + 3 different; or 2 the same + 2 different; or 3 the same + 1 different. (It cannot break down as 4 features the same + 0 different as the cards would be identical, and there are no identical cards in the Set deck.) 

This was my capstone project for high school informatics class in 2016.
The repository contains a client-server implementation of the game in C++ using the Qt framework.
The code is my final submission with minimal changes to make it build as of 2022.

Game completion is not implemented, there are several bugs and situations not accounted for, and code architecture was inspired by projects I had seen until then but is otherwise completely improvised and only clean enough for me to manage to implement the main features on time :smile:

## Building

To generate the executables run the following in each of the `Client` and `Server` folders:

```
$ qmake
$ make
```

You can also build the project with the Qt Creator IDE.

## Running the server

Successfully running the server should show the following logs:

```
[LOG] [<DATETIME>] Available IPs (<PUBLIC IP ADDRESSES>)
[LOG] [<DATETIME>] Listening for connections on port 2187
```

## Running clients (playing)

Starting a client opens an IP dialog where you should enter an appropriate IP address of a running server and connect.

<img src="https://user-images.githubusercontent.com/31972928/202913071-109cf4c7-1ab0-4ff4-a819-70f1eed93768.png" width="203px" />

You are then presented with a dialog with a username field and a list of game rooms on the server.
You can create or join a room with available space.

<img src="https://user-images.githubusercontent.com/31972928/202913176-9e29e29e-f660-4ac7-96a7-d3e61d275192.png" width="229px" />

Once in a room, you can immediately start searching for sets among the cards on the board.
When you believe you've found one, press the button to make your selection.
If you make a mistake, you cannot try again until another player makes a selection.
You also cannot press the button while another player is currently making their selection.

<img src="https://user-images.githubusercontent.com/31972928/202911976-0e304959-c43b-4ba8-9f7a-d84b36f6bb6b.png" width="800px" />
