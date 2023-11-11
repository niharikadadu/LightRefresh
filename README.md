# Light-Refresh

<h3> Project Description </h3>
The "Night Vision Timer" is a C program with a graphical user interface (GUI) built using the GTK+ library. This project combines timer functionality with screen setting control to reduce blue light exposure for a healthy screen experience for your eyes. The program allows users to keep track of time and experience a Blue-light filter effect after a set duration.

<h3>Features </h3>
<ul>
<li><b>Blink Mode : </b>The program will notify the user after specified time to take a break or distract his/her vision from screen for some time to ensure health of the eyes.</li>

<li><b>Blue Ray Guard Mode : </b>The program will automatically reduce the emission of blue rays on the screen for a specific amount of time.</li>

<li><b>Total Care Mode : </b>This mode is the combination of the Blink and Blue Ray Guard Mode which will notify the user and also reduce the emission of blue rays from the screen.</li>

<li><b>User Controls: </b>The user can interact with the program through the GUI, with options to start, pause, and reset the timer.</li>

<li><b>Display Total Time: </b>Upon exiting the program, it displays the total time elapsed and the time spent in Blue Filter mode in a user-friendly format of minutes and seconds.</li>
</ul>

## Getting Started

Follow these steps to use the "Night Vision Timer" on your Linux system.

### 1. Clone the Repository
Clone the project repository from GitHub to your local machine using the following command:

```bash
git clone https://github.com/niharikadadu/LightRefresh.git
```

### 2. Change to the Project Directory
Navigate to the project directory using the cd command:

```bash
cd LightRefresh
```

### 3. Run the Bash Script
Make the Bash script executable if it's not already:

```bash
chmod +x script.sh
```
Then, run the script to install the necessary libraries and start the "Night Vision Timer" program:

```bash
bash script.sh
```


## Run Locally

If the above method do not work for you, follow the steps given below.

### 1. Clone the Repository
```bash
  git clone https://github.com/niharikadadu/LightRefresh.git
```

### 2. Go to the project directory

```bash
  cd LightRefresh
```

### 3. Install dependencies

```bash
  sudo apt install libgtk-3-dev
```

### 4. Compile the .c file

```bash
  gcc `pkg-config --cflags gtk+-3.0` -o lightref lightref.c `pkg-config --libs gtk+-3.0`
```

### 5. Run the executable

```bash
    ./lightref
```

### Usage
The "LightRefresh" program will open with a GUI. You can use the provided buttons to start, pause, and reset the timer. After 5 minutes, you will experience the "Blue-light" effect on your screen. When you exit the program, it will display the total time elapsed and the time spent in "Blue-light" filter mode in minutes and seconds.

## Authors

- [Harsh Singh](https://github.com/harsh-s15)
- [Sai Kiran Reddy](https://github.com/KiRaN-110)
- [Samarth Patel](https://github.com/Samarth11203)
- [Niharika Dadu](https://github.com/niharikadadu)

