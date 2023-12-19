# pvz2024
In this factory, we will accomplish pvz2024

some basic funtion:

putimage(int x,int y, IMAGE *png)--put picture png at spot (x,y)

if we need invisible background picture, use putimagePNG

structs: sunshine zm bullet, they have variables x, y, frameindex, speed .... (depend on usage)

basically, each struct have 2 functions -- update function and image function, see the code "history" to get more information

gameInit will play a start video, well music are not added or I cannot coding on XuYong's class.

that's the introduction, read code to see more.

# Modifies to OOP -using classes and pointers
to adjust our code more readable and debugable, we pack them with classes.

item: plant, bullet, sunshine, zombies. they have same update funtion and image funtion.

game:gameInit and game update, collisions. All the interaction between classes will be in this .h
