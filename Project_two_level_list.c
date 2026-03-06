#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InnerNode
{
    char *data;
    struct InnerNode *next;
} InnerNode;

typedef struct OuterNode
{
    InnerNode *childHead;
    struct OuterNode *next;
} OuterNode;

InnerNode *createInnerNode(char *text)
{
    InnerNode *newNode = (InnerNode *)malloc(sizeof(InnerNode));
    newNode->data = strdup(text);
    newNode->next = NULL;
    return newNode;
}

OuterNode *createOuterNode()
{
    OuterNode *newNode = (OuterNode *)malloc(sizeof(OuterNode));
    newNode->childHead = NULL;
    newNode->next = NULL;
    return newNode;
}

OuterNode *buildTwoLevelList(const char *filename, int N)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: File not found!\n");
        return NULL;
    }

    OuterNode *head = NULL;
    OuterNode *currentOuter = NULL;
    char buffer[256];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = 0;

        if (head == NULL || count == N)
        {
            OuterNode *newOuter = createOuterNode();
            if (head == NULL)
            {
                head = newOuter;
            }
            else
            {
                currentOuter->next = newOuter;
            }
            currentOuter = newOuter;
            count = 0;
        }

        InnerNode *newInner = createInnerNode(buffer);

        if (currentOuter->childHead == NULL)
        {
            currentOuter->childHead = newInner;
        }
        else
        {
            InnerNode *temp = currentOuter->childHead;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newInner;
        }
        count++;
    }

    fclose(file);
    return head;
}

void printList(OuterNode *head)
{
    int i = 1;
    while (head)
    {
        printf("Group %d:\n", i++);
        InnerNode *inner = head->childHead;
        while (inner)
        {
            printf("  - %s\n", inner->data);
            inner = inner->next;
        }
        head = head->next;
    }
}

int countTotalElements(OuterNode *head) {
    int total = 0;
    while (head) {
        InnerNode *inner = head->childHead;
        while (inner) {
            total++;
            inner = inner->next;
        }
        head = head->next;
    }
    return total;
}

void freeList(OuterNode *head)
{
    while (head)
    {
        OuterNode *nextOuter = head->next;
        InnerNode *inner = head->childHead;
        while (inner)
        {
            InnerNode *nextInner = inner->next;
            free(inner->data);
            free(inner);
            inner = nextInner;
        }
        free(head);
        head = nextOuter;
    }
}

char* findLongestString(OuterNode *head) {
    char *longest = NULL;
    size_t maxLen = 0;

    while (head) {
        InnerNode *inner = head->childHead;
        while (inner) {
            size_t currentLen = strlen(inner->data);
            if (currentLen > maxLen) {
                maxLen = currentLen;
                longest = inner->data;
            }
            inner = inner->next;
        }
        head = head->next;
    }
    return longest;
}

char* findShortestString(OuterNode *head) {
    char *shortest = NULL;
    size_t minLen = (size_t)-1;

    while (head) {
        InnerNode *inner = head->childHead;
        while (inner) {
            size_t currentLen = strlen(inner->data);
            if (currentLen < minLen) {
                minLen = currentLen;
                shortest = inner->data;
            }
            inner = inner->next;
        }
        head = head->next;
    }
    return shortest;
}

int main()
{
    int N = 3;
    OuterNode *myList = buildTwoLevelList("data_for_two_level_list.txt", N);

    if (myList)
    {
        printf("Current List: ");
        printList(myList);

        int total = countTotalElements(myList);
        printf("Total elements in list: %d\n", total);

        char *longest = findLongestString(myList);
        if (longest) {
            printf("The longest word is: %s (length: %zu)\n", longest, strlen(longest));
        }

        char *shortest = findShortestString(myList);
        if (shortest) {
            printf("The shortest word is: %s (length: %zu)\n", shortest, strlen(shortest));
        }
    }
    printf("\nCleaning up memory...\n");
    freeList(myList);
    printf("Memory freed successfully.\n");
    myList = NULL;
    return 0;
}
