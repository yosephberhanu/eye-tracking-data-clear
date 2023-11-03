#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define no_participants 18

int splitLine(char* line, char split[20][20]) 
{
    int count = 0;
    int j = 0;
    int i = 0;
    while(line[i] && count < 20){
        if(line[i] == ' ' || line[i] == '\t') {
            if(j!=0){
                split[count][j++] = '\0';
                count++;
                j = 0;
            }
        }else {
            split[count][j++] = line[i];
        }
        i++;
    }
    return count+1;
}
void processfile(char* participant, char* inputFile, char* outputFile)
{
    printf("Processing file: %s, for participant %s, to file %s", inputFile, participant,outputFile);
    FILE * input;
    FILE * output;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char values[20][20];
    const char* DATA = "10";
    const char* CHANGE = "12";
    const char* START = "16";
    const char* END = "END";
    char stimuli[] = "-100";
    char pstimuli[] = "-100";
    char block[] = "-100";
    unsigned long seq = 0;
    input = fopen(inputFile, "r");
    output = fopen(outputFile, "w");
    if (input == NULL || output == NULL){
        fclose(input);
        fclose(output);
        printf("\n ******* Skipping: File not found for articipant: %s *********\n", participant);
        return;
    }
    // Save Header
    fprintf(output, "count,stimuli, block, total_time, delta_time, x_gaze, y_gaze, x_corrected_gaze, y_corrected_gaze, region, pupil_width, pupil_height, quality, fixation\n");

    while ((read = getline(&line, &len, input)) != -1) {
        // Split the line
        // Check if it starts with and
        

        int l = splitLine(line, values);
        if(l <= 0)   // No line found
        {
            continue;
        }

        if (strcmp(values[0], CHANGE)==0)
        {
            if(strcmp(values[l-1], END) >= 0){
                break;
            } else {
                strncpy ( stimuli, values[3], sizeof(stimuli) );
                strncpy ( block, values[4], sizeof(block) );
            }
        } else if (strcmp(values[0], DATA) == 0 && l == 13)
        {

            //(stimuli, block, total_time, delta_time, x_gaze, y_gaze, x_corrected_gaze, y_corrected_gaze, region, pupil_width, pupil_height, quality, fixation, count)   
            fprintf(output, "%zu, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n", seq++,stimuli, block, values[1],values[2],values[3],values[4],values[5],values[6],values[7],values[8],values[9],values[10],values[11]);
            // fprintf(output, "%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n", stimuli, block, values[1],values[2],values[3],values[4],values[5],values[6],values[7],values[8],values[9],values[10],values[11]);
        }
    }

    fclose(input);
    fclose(output);
    if (line)
        free(line);
    printf("-Done\n");
}

int main(){

    char* participants[] = { "300","301","302","305","306","310","311","314","315","320","322", "325","327","328","329","330",
                   "336","338","339","340","345","350","351","353","400","401","403","404","406","407",
                   "409","410","411","412","415","416","417"};
    // char* participants[] = { "345"};
    char* inputFileNameF = "../original/behavioral-data-c/%s/codereview-%s.txt";
    char* outputFileNameF = "../original/behavioral-data-c/%s/%s.csv";
    char inputFileName[255];
    char outputFileName[255];
    for (int i = 0; i < sizeof(participants)/8; ++i)
    {
        sprintf(inputFileName, inputFileNameF, participants[i], participants[i]);
        sprintf(outputFileName, outputFileNameF, participants[i], participants[i]);
        processfile(participants[i], inputFileName, outputFileName);
    }
    
    return 0;
}