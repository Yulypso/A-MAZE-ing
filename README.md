# RiMaze

## Author

[![Linkedin: Thierry Khamphousone](https://img.shields.io/badge/-Thierry_Khamphousone-blue?style=flat-square&logo=Linkedin&logoColor=white&link=https://www.linkedin.com/in/tkhamphousone/)](https://www.linkedin.com/in/tkhamphousone)

---

<br/>

## Setup

```bash
$ git clone https://github.com/Yulypso/RiMaze.git
$ cd RiMaze
```

---

<br/>

## Start RiMaze

### Run default Maze

```bash
$ make 
```

<br/>

### Some examples

```bash
$ make compile
$ ./RIMAZE -r 1 -i maze-esiea.txt
```
> Rimaze solving a loaded maze (maze-esiea.txt) at speed 1

<br/>

```bash
$ make compile
$ ./RIMAZE -g 20 20 -o my-maze.txt
```
> generate a 20x20 maze (my-maze.txt) 

```bash
$ make compile
$ ./RIMAZE -g 40 200 -o maze-40-200.txt -r 10000 -i maze-40-200.txt
```
> Rimaze solving a new generated 40x200 maze at speed 100000

<br/>

---

<br/>

## Manual

```bash
-- RiMaze arguments--

  -i <input file name>          Input file name, input file will be read from mazes/ directory
  -o <output file name>         Output file name, output file will be store in mazes/ directory
  -g <nb lines> <nb columns>    Generate maze with a specific size
  -r <speed>                    Rimaze solving the input maze with a specific speed (-1: maximum speed)
```

### generate Maze
```bash
$ make compile

# New generated maze will be stored as text file in mazes/ directory
$ ./RIMAZE -g <nb lines> <nb columns> -o <output file name>
```

### Rimaze solving maze
```bash
$ make compile

# Selected maze will be read from mazes/ directory
$ ./RIMAZE -r <speed> -i <input file name>
```

---

<br/>

## Specific mazes

```bash
$ make compile
$ ./RIMAZE -r 10 -i maze-blocked-exit.txt
```
> Rimaze solving a blocked exit

<br/>

```bash
$ make compile
$ ./RIMAZE -r 1 -i maze-blocked-entrance.txt
```
> Rimaze solving a blocked entrance

<br/>

```bash
$ make compile
$ ./RIMAZE -r 1 -i maze-empty-wall.txt
```
> Rimaze solving a maze without any wall

<br/>