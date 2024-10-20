#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../models/db_information_model.h"
#include "../models/feedback_model.h"
#include "../utils/constants.h"
#include "../utils/utils.h"

int getNewFeedbackId()
{
    int fd = open(dbInformationDatabase, O_RDONLY), id;
    DbInformationModel model;
    lockRecordDbInfo(fd, F_RDLCK);
    lseek(fd, 0, SEEK_SET);
    read(fd, &model, sizeof(model));

    id = model.feedbackCount + 1;

    lockRecordDbInfo(fd, F_UNLCK);
    close(fd);

    return id;
}

int addFeedback(int userId, const char *feedbackMessage)
{
    FeedbackModel feedback;
    int fd = open(feedbackDatabase, O_RDWR | O_APPEND | O_CREAT, 0600);
    if (fd == -1)
    {
        perror("Error opening feedback database");
        return -1;
    }

    // Lock the feedback database for writing
    if (lockFeedbackDb(fd, F_WRLCK) == -1)
    {
        perror("Error locking feedback database");
        close(fd);
        return -1;
    }

    // Populate feedback details
    feedback.feedback_id = getNewFeedbackId();
    feedback.user_id = userId;
    strncpy(feedback.feedback_message, feedbackMessage, sizeof(feedback.feedback_message) - 1);
    feedback.feedback_message[sizeof(feedback.feedback_message) - 1] = '\0';  // Ensure null termination

    // Write the feedback to the database
    if (write(fd, &feedback, sizeof(FeedbackModel)) == -1)
    {
        perror("Error writing to feedback database");
        lockFeedbackDb(fd, F_UNLCK);
        close(fd);
        return -1;
    }

    // Unlock and close the feedback database
    if (lockFeedbackDb(fd, F_UNLCK) == -1)
    {
        perror("Error unlocking feedback database");
    }

    close(fd);
    printf("Feedback added successfully with ID: %d\n", feedback.feedback_id);
    DbInformationModel model;
    int fd2 = open(dbInformationDatabase, O_RDWR);
    lockRecordDbInfo(fd2, F_WRLCK);
    if (fd2 < 0)
    {
        perror("Error opening user information database");
    }

    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        perror("Error seeking in user information database");
        close(fd2);
    }

    if (read(fd2, &model, sizeof(DbInformationModel)) != sizeof(DbInformationModel))
    {
        perror("Error reading from user information database");
        close(fd2);
    }

    model.feedbackCount++;

    // Write updated model back to the file
    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        perror("Error seeking to start of user information database");
        close(fd2);
    }

    if (write(fd2, &model, sizeof(DbInformationModel)) != sizeof(DbInformationModel))
    {
        perror("Error writing to user information database");
        close(fd2);
    }
    lockRecordDbInfo(fd2, F_UNLCK);
    close(fd2);
    return feedback.feedback_id;  // Return the feedback ID
}

// Function to list all feedbacks
void listAllFeedbacks()
{
    FeedbackModel feedback;
    int fd = open(feedbackDatabase, O_RDONLY, 0600);
    if (fd == -1)
    {
        perror("Error opening feedback database");
        return;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    printf("\n%-10s %-10s %-1024s\n", "Feedback ID", "User ID", "Feedback Message");
    printf("--------------------------------------------------------------------------------\n");

    // Read and display each feedback record
    while (read(fd, &feedback, sizeof(FeedbackModel)) == sizeof(FeedbackModel))
    {
        printf("%-10d %-10d %-1024s\n", feedback.feedback_id, feedback.user_id, feedback.feedback_message);
    }

    close(fd);
}

int compareFeedbackIds(const void *a, const void *b)
{
    FeedbackModel *feedbackA = (FeedbackModel *)a;
    FeedbackModel *feedbackB = (FeedbackModel *)b;
    return feedbackB->feedback_id - feedbackA->feedback_id; 
}

char *printAllFeedbacks()
{
     FeedbackModel feedback;
    int fd = open(feedbackDatabase, O_RDONLY, 0600);

    if (fd == -1)
    {
        perror("Error opening feedback database");
        return NULL;
    }

    // Lock the feedback database for reading
    if (lockFeedbackDb(fd, F_RDLCK) == -1)
    {
        perror("Error locking feedback database");
        close(fd);
        return NULL;
    }

    // Move to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    size_t initial_size = 2048; // Start with a larger initial size
    size_t current_size = initial_size;
    size_t content_length = 0;

    // Dynamically allocate memory for the string
    char *str = (char *)malloc(initial_size * sizeof(char));
    if (str == NULL)
    {
        perror("Failed to allocate memory");
        lockFeedbackDb(fd, F_UNLCK); // Unlock before returning
        close(fd);
        return NULL;
    }

    // Initialize the buffer
    str[0] = '\0'; // Set the string to be empty initially

    // Append header to the buffer
    content_length += snprintf(str + content_length, current_size - content_length,
                               "\n%-15s %-15s %-s\n", 
                               "FeedbackID", "UserID", "FeedbackMessage");

    // Append the separator
    content_length += snprintf(str + content_length, current_size - content_length,
                               "-------------------------------------------------------------------------------\n");

    size_t feedback_count = 0;
    size_t feedback_capacity = 10; // Initial capacity
    FeedbackModel *feedbacks = (FeedbackModel *)malloc(feedback_capacity * sizeof(FeedbackModel));
    if (feedbacks == NULL)
    {
        perror("Failed to allocate memory for feedbacks");
        free(str);
        lockFeedbackDb(fd, F_UNLCK); // Unlock before returning
        close(fd);
        return NULL;
    }

    // Loop through the feedbacks and collect all records
    while (read(fd, &feedback, sizeof(FeedbackModel)) == sizeof(FeedbackModel))
    {
        // Ensure there is enough space to store the new feedback
        if (feedback_count >= feedback_capacity)
        {
            feedback_capacity *= 2;
            feedbacks = (FeedbackModel *)realloc(feedbacks, feedback_capacity * sizeof(FeedbackModel));
            if (feedbacks == NULL)
            {
                perror("Failed to reallocate memory for feedbacks");
                free(str);
                lockFeedbackDb(fd, F_UNLCK); // Unlock before returning
                close(fd);
                return NULL;
            }
        }
        feedbacks[feedback_count++] = feedback;
    }

    // Unlock the feedback database
    lockFeedbackDb(fd, F_UNLCK);
    close(fd);

    // Sort the feedbacks by feedback ID (if necessary)
    qsort(feedbacks, feedback_count, sizeof(FeedbackModel), compareFeedbackIds);

    // Append sorted feedbacks to the buffer
    for (size_t i = 0; i < feedback_count; i++)
    {
        FeedbackModel f = feedbacks[i];

        // Ensure there is enough space in the string buffer to append the new feedback data
        size_t needed_size = content_length + 100 + strlen(f.feedback_message);  // Estimate required space
        if (needed_size >= current_size)
        {
            current_size = needed_size * 2; // Double the buffer size based on the need
            str = (char *)realloc(str, current_size * sizeof(char));
            if (str == NULL)
            {
                perror("Failed to reallocate memory");
                free(feedbacks);
                return NULL;
            }
        }

        // Append feedback details to the buffer without truncating
        content_length += snprintf(str + content_length, current_size - content_length,
                                   "%-15d %-15d %-s\n",
                                   f.feedback_id,
                                   f.user_id,
                                   f.feedback_message);
    }

    // Free the feedbacks array
    free(feedbacks);

    return str;
}


#endif