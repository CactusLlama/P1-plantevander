struct Plants {    
    char name[20];
    float airTempMax;
    float airTempMin;
    float waterTempMax;
    float waterTempMin;
    float ecMax;
    float ecMin;
    float phMax;
    float phMin;
    float humidMax;
    float humidMin;
};

typedef struct PlantSensors {
    char name[20];
    float ph;
    float ec;
    float waterLevel;
    float airTemp;
    float waterTemp;
    float humidity;
} curPlant;