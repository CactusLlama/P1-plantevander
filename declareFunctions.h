//look at user input and take action based on it
void process_input(char a, struct Plants plants[], int *plantAmount, struct PlantSensors *curPlant);

//put a delay in code (code stops for mSeconds)
void delay(int mSeconds);

//print that input is invalid
void invalid_input();

//read data from .txt file and put it into struct
void read_data(struct PlantSensors *current);

//put data from struct into .txt file
void overwrite_data(struct PlantSensors *current);

//edit the values in struct
void edit_data(struct PlantSensors *current, int sprinklerState, int fanState, int waterHeatState);

//print all values for chosen plant
void print_plant(struct Plants current);

//list all plants found in array of structures
void list_plants(struct Plants plants[], int plantAmount);

//predefine the values in first struct in the array
void init_structs(struct Plants plants[]);

//enter editing mode
void edit_mode(struct Plants plants[], int *plantAmount);  

//edit the limit values for a chosen plant in the array of structs 
void edit_plant(struct Plants *current);

//enter debug mode
//allows the user to edit the values read during runtime
//used during presentation such that restarting program isn't necessary
void debug_mode(struct PlantSensors *current);

//compares two numbers and returns 1 or 0. used a bunch in edit_plant()
int cmp_nums(float a, float b);