#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InnerNode
{
    char* data;
    struct InnerNode* next;
} InnerNode;

typedef struct OuterNode
{
    InnerNode* childHead;
    struct OuterNode* next;
} OuterNode;

InnerNode* createInnerNode(char* text)
{
    InnerNode* newNode = (InnerNode*)malloc(sizeof(InnerNode));
    newNode->data = strdup(text);
    newNode->next = NULL;
    return newNode;
}

OuterNode* createOuterNode()
{
    OuterNode* newNode = (OuterNode*)malloc(sizeof(OuterNode));
    newNode->childHead = NULL;
    newNode->next = NULL;
    return newNode;
}

OuterNode* buildTwoLevelList(const char* filename, int N)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: File not found!\n");
        return NULL;
    }

    OuterNode* head = NULL;
    OuterNode* currentOuter = NULL;
    char buffer[256];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = 0;

        if (head == NULL || count == N)
        {
            OuterNode* newOuter = createOuterNode();
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

        InnerNode* newInner = createInnerNode(buffer);

        if (currentOuter->childHead == NULL)
        {
            currentOuter->childHead = newInner;
        }
        else
        {
            InnerNode* temp = currentOuter->childHead;
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

void printList(OuterNode* head)
{
    int i = 1;
    while (head)
    {
        printf("Group %d:\n", i++);
        InnerNode* inner = head->childHead;
        while (inner)
        {
            printf("  - %s\n", inner->data);
            inner = inner->next;
        }
        head = head->next;
    }
}

int countTotalElements(OuterNode* head) {
    int total = 0;
    while (head) {
        InnerNode* inner = head->childHead;
        while (inner) {
            total++;
            inner = inner->next;
        }
        head = head->next;
    }
    return total;
}

int countGroups(OuterNode* head) {
    int groups = 0;
    while (head) {
        groups++;
        head = head->next;
    }
    return groups;
}

char* findLongestString(OuterNode* head) {
    char* longest = NULL;
    size_t maxLen = 0;

    while (head) {
        InnerNode* inner = head->childHead;
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

char* findShortestString(OuterNode* head) {
    char* shortest = NULL;
    size_t minLen = (size_t)-1;

    while (head) {
        InnerNode* inner = head->childHead;
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

char* findBySubstring(OuterNode* head, const char* sub) {
    while (head) {
        InnerNode* inner = head->childHead;
        while (inner) {
            if (strstr(inner->data, sub))
                return inner->data;
            inner = inner->next;
        }
        head = head->next;
    }
    return NULL;
}

void reverseInnerList(OuterNode* node) {
    InnerNode* prev = NULL;
    InnerNode* curr = node->childHead;
    while (curr) {
        InnerNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    node->childHead = prev;
}

void addToGroup(OuterNode* head, int group, const char* text) {
    int i = 1;
    while (head && i < group) {
        head = head->next;
        i++;
    }
    if (!head) return;
    InnerNode* newNode = createInnerNode((char*)text);
    newNode->next = head->childHead;
    head->childHead = newNode;
}

void printReverse(OuterNode* head) {
    if (!head) return;
    printReverse(head->next);
    InnerNode* inner = head->childHead;
    while (inner) {
        printf("%s ", inner->data);
        inner = inner->next;
    }
    printf("\n");
}

void freeList(OuterNode* head)
{
    while (head)
    {
        OuterNode* nextOuter = head->next;
        InnerNode* inner = head->childHead;
        while (inner)
        {
            InnerNode* nextInner = inner->next;
            free(inner->data);
            free(inner);
            inner = nextInner;
        }
        free(head);
        head = nextOuter;
    }
}

int main()
{
    int N = 3;
    OuterNode* myList = buildTwoLevelList("data_for_two_level_list.txt", N);

    if (myList)
    {
        printf("Current List:\n");
        printList(myList);

        int total = countTotalElements(myList);
        printf("Total elements: %d\n", total);

        printf("Groups: %d\n", countGroups(myList));

        char* longest = findLongestString(myList);
        if (longest) printf("Longest: %s\n", longest);

        char* shortest = findShortestString(myList);
        if (shortest) printf("Shortest: %s\n", shortest);

        char* found = findBySubstring(myList, "ab");
        if (found) printf("Found substring: %s\n", found);

        addToGroup(myList, 1, "InsertedWord");
        reverseInnerList(myList);

        printf("After modifications:\n");
        printList(myList);

        printf("Reverse print:\n");
        printReverse(myList);
    }

    freeList(myList);
    return 0;
}
