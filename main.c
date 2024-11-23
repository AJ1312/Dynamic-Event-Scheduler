#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Event {
    char name[100];
    int startTime;  
    int duration;  
    int priority;   
};

struct Event schedule[100];
int eventCount = 0;

void addEvent();
void viewEvents();
int checkConflict(struct Event newEvent);
void resolveConflict(struct Event *newEvent);
void saveSchedule();
void loadSchedule();
void sortEvents();

int main() {
    int choice;
    loadSchedule();

    while (1) {
        printf("\nEvent Scheduler\n");
        printf("1. Add Event\n");
        printf("2. View Events\n");
        printf("3. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addEvent(); break;
            case 2: viewEvents(); break;
            case 3: saveSchedule(); return 0;
            default: printf("Invalid choice.\n");
        }
    }
}

void addEvent() {
    struct Event newEvent;
    printf("Enter event name: ");
    scanf(" %[^\n]", newEvent.name);
    printf("Enter start time (in minutes from midnight): ");
    scanf("%d", &newEvent.startTime);
    printf("Enter duration (in minutes): ");
    scanf("%d", &newEvent.duration);
    printf("Enter priority (1 for high priority): ");
    scanf("%d", &newEvent.priority);

    if (checkConflict(newEvent)) {
        printf("Conflict detected! Resolving...\n");
        resolveConflict(&newEvent);
    }

    schedule[eventCount++] = newEvent;
    sortEvents();
    printf("Event added successfully.\n");
}

void viewEvents() {
    if (eventCount == 0) {
        printf("No events scheduled.\n");
        return;
    }

    printf("\nScheduled Events:\n");
    for (int i = 0; i < eventCount; i++) {
        printf("Event: %s | Start Time: %d | Duration: %d mins | Priority: %d\n",
               schedule[i].name, schedule[i].startTime, schedule[i].duration, schedule[i].priority);
    }
}

int checkConflict(struct Event newEvent) {
    for (int i = 0; i < eventCount; i++) {
        int existingEndTime = schedule[i].startTime + schedule[i].duration;
        int newEventEndTime = newEvent.startTime + newEvent.duration;
        
        if (newEvent.startTime < existingEndTime && newEventEndTime > schedule[i].startTime) {
            return 1;  // Conflict found
        }
    }
    return 0;  // No conflict
}

void resolveConflict(struct Event *newEvent) {
    int highestPriority = 10;  // Default to a low priority number for high priority event

    for (int i = 0; i < eventCount; i++) {
        if (schedule[i].priority < highestPriority) {
            highestPriority = schedule[i].priority;
        }
    }

    if (newEvent->priority > highestPriority) {
        printf("New event has higher priority. Rescheduling...\n");
        newEvent->startTime = schedule[eventCount - 1].startTime + schedule[eventCount - 1].duration;
    } else {
        printf("New event has lower priority. Adjusting time...\n");
        newEvent->startTime = schedule[eventCount - 1].startTime + schedule[eventCount - 1].duration;
    }
}

void saveSchedule() {
    FILE *file = fopen("schedule.txt", "w");
    for (int i = 0; i < eventCount; i++) {
        fprintf(file, "%s %d %d %d\n", schedule[i].name, schedule[i].startTime, schedule[i].duration, schedule[i].priority);
    }
    fclose(file);
    printf("Schedule saved successfully.\n");
}

void loadSchedule() {
    FILE *file = fopen("schedule.txt", "r");
    if (!file) return;

    while (fscanf(file, " %[^\n] %d %d %d", schedule[eventCount].name, &schedule[eventCount].startTime, 
                    &schedule[eventCount].duration, &schedule[eventCount].priority) != EOF) {
        eventCount++;
    }
    fclose(file);
    printf("Schedule loaded successfully.\n");
}

void sortEvents() {
    for (int i = 0; i < eventCount - 1; i++) {
        for (int j = 0; j < eventCount - i - 1; j++) {
            if (schedule[j].startTime > schedule[j + 1].startTime) {
                struct Event temp = schedule[j];
                schedule[j] = schedule[j + 1];
                schedule[j + 1] = temp;
            }
        }
    }
}
