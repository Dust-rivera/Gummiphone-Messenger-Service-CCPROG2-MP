/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying
the concepts learned. I have constructed the functions and their respective algorithms and corresponding
code by myself. The program was run, tested, and debugged by my own efforts. I further certify that I
have not copied in part or whole or otherwise plagiarized the work of other students and/or persons.
Dustine Gian Rivera, DLSU ID#12414921
Diana Angela Ramirez, DLSU ID#12415278
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
typedef char String[51];
#define MAX_USERS 30 //Maximum no. of Users that can be registered
#define MAX_MESSAGES 1000 // Maximum no. of Messages that can exist in the program:
#define MAX_MESSAGE_LENGTH 1000 // Maximum no. of Characters for each message / announcements
#define MAX_PASSWORD_LENGTH 21 // Maximum no. of Characters for passwords
#define MAX_SECURITY_LENGTH 101 // Maximum no. of Characters for Security Questions & Answers
#define MAX_PERSONAL_CONNECTIONS 10 // Maximum no. of Personal connections

struct accounts
{
String usernames;
String names;
char passwords[MAX_PASSWORD_LENGTH];
char security_answers[MAX_SECURITY_LENGTH];
char security_questions[MAX_SECURITY_LENGTH];
char description[MAX_MESSAGES];
String personal_connections[10];
String lock;
String request;
};

struct message 
{
String sender;
String recipient;
String subject;
char body[MAX_MESSAGE_LENGTH];

};

/*  
 * main_menu displays the main menu options and gets user input  
 * @return choice - the integer corresponding to the user's menu selection  
 * Pre-condition: User must enter a valid integer input  
 */  
int main_menu(){
int choice;
	printf("\n===== MAIN PAGE =====\n");
    printf("1. Log in Menu\n");
    printf("2. Administrator module\n");
    printf("3. Exit\n");
    printf("======================\n");
	scanf("%d", &choice);
	fflush(stdin);
	
return choice;
}

/*clean removes the newline character from a string
@param source - pointer to a null-terminated string that may have a newline at the end
@return void (function does not return a value)
pre-condition: source must be a valid, null-terminated string with at least one character*/
void clean(char *source)
{
	if (source[strlen(source)-1] == '\n')
		source[strlen(source)-1] = '\0';
}

/* Search returns the index of the account with a matching username
@param user - array of account structures containing the username field
@param temp_username - the string containing the username to search for
@param user_counter - integer representing the total number of accounts in the user array
@return returns the index of the matching account if found, otherwise returns -1
pre-condition: the user array is valid with user_counter elements, and temp_username is a valid null-terminated string*/
int Search(struct accounts user[], String temp_username, int user_counter){
	for(int i = 0; i < user_counter; i++){
		if(strcmp(user[i].usernames,temp_username) == 0){
			return i;
		}
	}
	return -1; 
}

/*updateUsernameInFiles updates all occurrences of an old username to a new username
in both "personal_connections.txt" and "messages.txt".
@param oldUsername - a constant character pointer representing the username to replace
@param newUsername - a constant character pointer representing the new username
@return void (function does not return a value)
pre-condition: "personal_connections.txt" and "messages.txt" must exist and be properly formatted*/
void updateUsernameInFiles(const char *oldUsername, const char *newUsername) {
    FILE *file1 = fopen("personal_connections.txt", "r");
    FILE *file2 = fopen("messages.txt", "r");
    FILE *temp1 = fopen("temp_personal.txt", "w");
    FILE *temp2 = fopen("temp_messages.txt", "w");

    if (!file1 || !file2 || !temp1 || !temp2) {
        printf("Error opening files!\n");
        return;
    }

    char line[1200];
    int found1 = 0, found2 = 0;

    // Update personal_connections.txt
    while (fgets(line, sizeof(line), file1)) {
        char updatedLine[1200];
        char *pos = line;
        found1 = 0;

        while ((pos = strstr(pos, oldUsername))) {
            found1 = 1;
            strncpy(updatedLine, line, pos - line);
            updatedLine[pos - line] = '\0';
            strcat(updatedLine, newUsername);
            strcat(updatedLine, pos + strlen(oldUsername));
            strcpy(line, updatedLine);
            pos += strlen(newUsername);
        }
        fprintf(temp1, "%s", line);
    }
    fclose(file1);
    fclose(temp1);
    remove("personal_connections.txt");
    rename("temp_personal.txt", "personal_connections.txt");

    // Update messages.txt
    while (fgets(line, sizeof(line), file2)) {
        char sender[50], recipient[50], subject[100], body[1000];
        if (sscanf(line, "%49[^|]|%49[^|]|%99[^|]|%999[^\n]", sender, recipient, subject, body) == 4) {
            if (strcmp(sender, oldUsername) == 0) {
                strcpy(sender, newUsername);
                found2 = 1;
            }
            if (strcmp(recipient, oldUsername) == 0) {
                strcpy(recipient, newUsername);
                found2 = 1;
            }
            fprintf(temp2, "%s|%s|%s|%s\n", sender, recipient, subject, body);
        } else {
            fprintf(temp2, "%s", line);
        }
    }
    fclose(file2);
    fclose(temp2);
    remove("messages.txt");
    rename("temp_messages.txt", "messages.txt");

    if (found1 || found2) {
        printf("Username updated successfully.\n");
    } else {
        printf("No occurrences found.\n");
    }
}

/*save_accounts writes account information and personal connections to external files
@param user - array of account structures containing account data (usernames, names, passwords, 
security questions, security answers, description, lock status, and request)
@param user_counter - integer representing the total number of accounts in the user array
@return void (function does not return a value)
pre-condition: the user array must contain valid account data for user_counter elements; "accounts.txt" and personal connection
files are writable, and each user's personal_connections array is properly null-terminated*/ 
void save_accounts(struct accounts user[], int user_counter) {
    int exit_flag = 0;
    
    FILE *file = fopen("accounts.txt", "wt");
    FILE *file2 = fopen("personal_connections.txt", "wt");

    if (file == NULL || file2 == NULL) {
        printf("Error opening file(s)!\n");
        exit_flag = 1;
    }

    if (!exit_flag) {
        for (int i = 0; i < user_counter && !exit_flag; i++) {
            if (fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s\n",
                        user[i].usernames, user[i].names, user[i].passwords,
                        user[i].security_questions, user[i].security_answers,
                        user[i].description, user[i].lock, user[i].request) < 0) {
                exit_flag = 1;
            }

            if (!exit_flag) {
                if (fprintf(file2, "%s", user[i].usernames) < 0) {
                    exit_flag = 1;
                }
                for (int j = 0; user[i].personal_connections[j][0] != '\0' && !exit_flag; j++) {
                    if (fprintf(file2, "|%s", user[i].personal_connections[j]) < 0) {
                        exit_flag = 1;
                    }
                }
                if (!exit_flag && fprintf(file2, "\n") < 0) {
                    exit_flag = 1;
                }
            }
        }
    }

    if (file) fclose(file);
    if (file2) fclose(file2);

    if (exit_flag) {
        printf("Error saving account data.\n");
    }
}

/*compose_message allows the current user to compose and send a message
@param user - array of account structures containing user information and personal connections
@param user_index - the index of the currently logged in user in the user array
@param user_counter - the total number of users in the user array
@return void (function does not return a value)
pre-condition: the user array must be valid with at least user_counter elements; auxiliary functions 
such as clean() and Search() must be implemented; "messages.txt" must be writable*/
void compose_message(struct accounts user[], int user_index, int user_counter) {
    struct message msg;
    int choice, recipient_count = 0, exit = 0;//, lines (unused variable rin)
    char recipients[10][51];

    while (!exit) {  // message menu
        printf("\n===== COMPOSE MESSAGE =====\n");
        printf("Select message type:\n");
        printf("1. Personal Message\n");
        printf("2. Group Message\n");
        printf("3. Announcement\n");
        printf("4. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);// user choice
        fflush(stdin);

        switch (choice) {
            case 1: // for personal message 
               do{
                	printf("Enter username for recipient: ");
                    fgets(msg.recipient, sizeof(msg.recipient), stdin);//reads username
                    clean(msg.recipient);
                    
                    if(Search(user,msg.recipient, user_counter) == -1){//validate username
                    	printf("Please enter a valid username\n");// if not valid
					}	
					
					}while(Search(user,msg.recipient, user_counter) == -1);
					break;// loop until username is valid

            case 2: //send to all personal connections
             	if (user[user_index].personal_connections[0][0] == '\0'){//checks if user have connections
             		printf("No personal connections found\n");
             		choice = 5; //invalid option
				 } else {// copies each personal connection into the recipients array and counts
				 	for(int i=0;user[user_index].personal_connections[i][0] !='\0';i++){
				 		strcpy(recipients[i],user[user_index].personal_connections[i]);
				 		recipient_count++;
					 }
				 }
                break;

            case 3:  //announcement: set recipient to "all" for a broadcast message
                strcpy(msg.recipient, "all");
                break;

            case 4: //exit message composition
                exit = 1;
                printf("Returning to the previous menu...\n");
    			system("cls");
                break;

            default:
                printf("Invalid option. Please try again.\n");
        }
        
        if (!exit) {//if user did not choose to exit, proceed with composing the message
            printf("Enter subject: ");
            fgets(msg.subject, sizeof(msg.subject), stdin);
            clean(msg.subject);
            
            msg.body[0] = '\0'; 
            printf("Enter your message:\n");
            fgets(msg.body, sizeof(msg.body), stdin);
            clean(msg.body);

            strcpy(msg.sender, user[user_index].usernames);

            FILE *file = fopen("messages.txt", "a");// open the file "messages.txt" in append mode to store the message
            if (file == NULL) {
                printf("Error saving message! The message was not stored.\n");
                exit = 1;
            }

            if (choice == 1 || choice == 3) { 
                fprintf(file, "%s|%s|%s|%s\n", msg.sender, msg.recipient, msg.subject, msg.body);
            } else if (choice == 2) { // group messages, iterate over all recipients and write each as a separate entry
                for (int i = 0; i < recipient_count; i++) {
                    fprintf(file, "%s|%s|%s|%s\n", msg.sender, recipients[i], msg.subject, msg.body);
                }
            }

            fclose(file);
            system("cls");
            printf("Message sent successfully!\n");	
        }
    }
}


/*reply composes and sends a reply to an existing message by addressing group replies and direct responses
@param user - array of account structures containing user information
@param user_index - the index of the currently logged in user in the user array
@param msg - the original message to which the user is replying
@return void (function does not return a value)
pre-condition: the user array and msg structure must be valid; "messages.txt" must be accessible for reading and appending*/
void reply(struct accounts user[], int user_index, struct message msg) {
    struct message reply_msg;
    FILE *file;

    strcpy(reply_msg.sender, user[user_index].usernames);
    sprintf(reply_msg.subject, "User \"%s\" replied to Subject \"%s\"", reply_msg.sender, msg.subject);
    reply_msg.body[0] = '\0';

    printf("Enter your reply:\n");
    fgets(reply_msg.body, sizeof(reply_msg.body), stdin);// read the reply from standard input
    clean(reply_msg.body);

    file = fopen("messages.txt", "a");
    if (file != NULL) {
        
        FILE *file2 = fopen("messages.txt", "r");
        struct message temp_msg;

        if (file2 != NULL) {//looop thru each message in the file
            while (fscanf(file2, "%50[^|]|%50[^|]|%99[^|]|%999[^\n]\n", temp_msg.sender, temp_msg.recipient, temp_msg.subject, temp_msg.body) != EOF) {
        
                if ((strcmp(temp_msg.subject, msg.subject) == 0 &&
                    strcmp(temp_msg.body, msg.body) == 0 &&
                    strcmp(temp_msg.recipient, user[user_index].usernames) != 0)
					|| (strcmp(temp_msg.recipient,"all") == 0&&
                    strcmp(temp_msg.sender, user[user_index].usernames) != 0)) {

                    fprintf(file, "%s|%s|%s|%s\n", reply_msg.sender, temp_msg.recipient, reply_msg.subject, reply_msg.body);
                }
            }
            fclose(file2);

            if (strcmp(msg.sender, user[user_index].usernames) != 0) {// if the original sender is not the current user, send a direct reply to the original sender
                fprintf(file, "%s|%s|%s|%s\n", reply_msg.sender, msg.sender, reply_msg.subject, reply_msg.body);
            }
        } else {// if the file cannot be opened for reading
            printf("Error reading messages for group reply.\n");
        }

        fclose(file);
        printf("Reply sent successfully!\n");
    } else {//if the file cannot be opened for appending
        printf("Error saving reply. The reply was not stored.\n");
    }
}


/*view_announcements displays all announcements to the current user, allowing navigation and replies
@param user - array of account structures containing user information
@param user_index - the index of the currently logged in user in the user array
@param user_counter - the total number of users in the user array
@return void (function does not return a value)
Pre-condition: the user array is valid with at least user_counter elements; "messages.txt" is accessible;
announcement messages have "all" as the recipient*/
void view_announcements(struct accounts user[], int user_index, int user_counter) {
    FILE *file = fopen("messages.txt", "r");
    struct message messages[100];
    int message_count = 0, current_index = 0, choice = 0, loop = 1, file_exists = 1;

    if (file == NULL) {
        printf("No announcements found.\n");
        file_exists = 0;
    }

    if (file_exists) {//read through the file and load announcement messages into the messages array
     
        while (fscanf(file, "%50[^|]|%50[^|]|%99[^|]|%999[^\n]\n", messages[message_count].sender, messages[message_count].recipient, messages[message_count].subject, messages[message_count].body) != EOF) {
            if (strcmp(messages[message_count].recipient, "all") == 0) {//count messages that are announcements (recipient equals "all")
                message_count++;
            }
        }
        fclose(file);
    }

    if (message_count == 0 && file_exists) {//disable loop if no announcement
        printf("No more announcements.\n");
        loop = 0;
    }

    while (loop) {// main loop displaying announcement
        struct message msg = messages[current_index];
        int sender_index = Search(user, msg.sender, user_counter);

        system("cls");
        printf("From: %s (@%s)\nSubject: %s\nMessage: %s\n", user[sender_index].names, msg.sender, msg.subject, msg.body);
        printf("-------------------------------\n");

        printf("1. Reply\n2. Next Announcement\n3. Previous Announcement\n4. Back\nEnter choice: ");
        scanf("%d", &choice);
        fflush(stdin);

        if (choice == 1) {
            reply(user, user_index, msg);//reply function to send announcement
        } else if (choice == 2 && current_index < message_count - 1) {// move to next announcement
            current_index++;
        } else if (choice == 3 && current_index > 0) {// move to previous
            current_index--;
        } else if (choice == 4) {
            loop = 0;
        }
    }

    system("cls");
}


/*view_inbox displays all messages from the current user's inbox, allows navigation through messages, and provides an option to reply
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of users in the user array
@return void (function does not return a value)
pre-condition: the user array must be valid; "messages.txt" must be accessible;
messages are formatted correctly with the current user's username as the recipient*/ 

void view_inbox(struct accounts user[], int user_index, int user_counter) {
    FILE *file = fopen("messages.txt", "r");
    struct message messages[100];
    int message_count = 0, current_index = 0, choice = 0, loop = 1, file_exists = 1;// removed found = 0, unused var daw

    if (file == NULL) {
        file_exists = 0;
    }

    if (file_exists) {//read messages addressed to the current user from the file
     
        while (fscanf(file, "%50[^|]|%50[^|]|%99[^|]|%999[^\n]\n", messages[message_count].sender, messages[message_count].recipient, messages[message_count].subject, messages[message_count].body) != EOF) {
            if (strcmp(messages[message_count].recipient, user[user_index].usernames) == 0) {
                message_count++;
            }
        }
        fclose(file);
    }

    if (message_count == 0 && file_exists) {
        printf("No messages found.\n");
        loop = 0;
    }

    while (loop) {//main loop for inbox
        struct message msg = messages[current_index];
        int sender_index = Search(user, msg.sender, user_counter);

        system("cls");
        //display all details
        printf("From: %s (@%s)\nSubject: %s\nMessage: %s\n", user[sender_index].names, msg.sender, msg.subject, msg.body);

      
        FILE *temp_file = fopen("messages.txt", "r");
        struct message temp_msg;//scanning additional recipients
        int first = 1;

        printf("Other Recipients: ");
         //temp message match the current message, recipient is different from current user
        while (fscanf(temp_file, "%50[^|]|%50[^|]|%99[^|]|%999[^\n]\n", temp_msg.sender, temp_msg.recipient, temp_msg.subject, temp_msg.body) != EOF) {
            if (strcmp(msg.subject, temp_msg.subject) == 0 && strcmp(msg.body, temp_msg.body) == 0 &&
                strcmp(temp_msg.recipient, user[user_index].usernames) != 0 &&
                strcmp(temp_msg.recipient, msg.recipient) != 0) {

                if (!first) printf(", ");// comma separation
                printf("@%s", temp_msg.recipient);// prints username of other recipients
                first = 0;
            }
        }
        fclose(temp_file);

        if (first) printf("None");

        printf("\n-------------------------------\n");

        printf("1. Reply\n2. Next Message\n3. Previous Message\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);
        fflush(stdin);
	// Process the user's choice from the menu
        if (choice == 1) {
            reply(user, user_index, msg);//reply function for the current message
        } else if (choice == 2 && current_index < message_count - 1) {// Move to the next message if available
            current_index++;
        } else if (choice == 3 && current_index > 0) {// Move to the previous message if available
            current_index--;
        } else if (choice == 4) {
            loop = 0;
        }
    }

    system("cls");//exxit loop
}




/*modify_personal_info allows the current user to update their name or description after password confirmation
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of accounts in the user array
@return void (function does not return a value)
pre-condition: the user array must be valid with at least user_counter elements; functions clean() and save_accounts() are implemented*/ 

void modify_personal_info(struct accounts user[], int user_index, int user_counter){
int exit = 0;
int choice;
String temp_name, temp_description,temp_password;
while(!exit){ //main loop 
	system("cls");
	printf("Modify:\n");
	printf("=======================\n");
	printf("1. Name \n");
	printf("2. Description\n");
	printf("3. Back\n");
	printf("=======================\n");
	printf("Enter choice: ");
        scanf("%d", &choice);
        fflush(stdin);
        system("cls");
        
       switch (choice) {// user chose to modify their name
            case 1: 
            printf("Enter new name:\n");
            scanf("%s", temp_name);//read the new name into temporary variable
        	fflush(stdin); //flush the input buffer after reading
            break;

            case 2: //user chose to modify their description

            printf("Enter new description:\n");
            scanf("%s", temp_description);//read the new description into temporary variable
        	fflush(stdin);//flush the input buffer after reading
            break;

            case 3: //user chose to go back
            	exit = 1;
                printf("Returning to the previous menu...\n");
    			system("cls");
                break;

            default://invalid option selected by the user
                printf("Invalid option. Please try again.\n");
        }
        
        //proceed with password confirmation
        if (choice == 1 || choice == 2){
        do{
        	
        printf("Enter password to confirm: "); // password	
		fgets(temp_password,sizeof(temp_password),stdin);//read the confirmation password
		clean(temp_password);
		fflush(stdin);	
		 //if the entered password does not match the stored password, prompt again
		if(strcmp(temp_password,user[user_index].passwords) != 0){
			printf("Please enter password again\n");
		}
		
		}while(strcmp(temp_password,user[user_index].passwords) != 0);
      	
		//updates the changes
        if(strcmp(temp_password,user[user_index].passwords) == 0)	{
        	if (choice == 1){
        		strcpy(user[user_index].names,temp_name);////update the user's name with the new 
        		
			}
			if (choice == 2){//update the user's description with the new 
				strcpy(user[user_index].description,temp_description);
			}
			system("cls");
        	printf("Changes Saved!\n ");
        	save_accounts(user, user_counter); //save all changes in the txt file
        	exit = 1;
		} 
		
		}
		
}

}




/*modify_account_security allows the current user to update their account security settings by changing their password or security question answer
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of accounts in the user array
@return void (function does not return a value)
Pre-condition: the user array must be valid with at least user_counter elements; functions clean() and save_accounts() are implemented; user input is correctly formatted*/

void modify_account_security(struct accounts user[], int user_index, int user_counter) 
{
    String old_password, new_password, confirm_password, new_security_answer, confirm_security_answer;
    int choice;
    int exit = 0;

    while (!exit) //loop 
	{
        printf("\n========== MODIFY ACCOUNT SECURITY ==========\n");
        printf("1. Change Password\n");
        printf("2. Change Security Question Answer\n");
        printf("3. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        fflush(stdin);
        system("cls");

        switch (choice) 
		{
            case 1: // change pass
                do 
				{
                    printf("Enter your current password: ");
                    fgets(old_password, sizeof(old_password), stdin);
                    clean(old_password);

                    if (strcmp(user[user_index].passwords, old_password) != 0)//inform user of incorrect password
					 {
                        printf("Incorrect password. Try again.\n");
                    }
                   //prompt until new pass is confirmed 
                }
                
				while (strcmp(user[user_index].passwords, old_password) != 0);

                do 
				{
                    printf("Enter new password: ");
                    fgets(new_password, sizeof(new_password), stdin);//read new pass
                    clean(new_password);

                    printf("Re-enter new password: ");
                    fgets(confirm_password, sizeof(confirm_password), stdin);//read confirmation of new password
                    clean(confirm_password);

                    if (strcmp(new_password, confirm_password) != 0) // if incorrect
					{
                        printf("Passwords do not match. Please try again.\n");
                    }
                }
				while (strcmp(new_password, confirm_password) != 0);
				//update the user's password and save changes
                strcpy(user[user_index].passwords, new_password);
                save_accounts(user, user_counter);
                system("cls");
                printf("Password updated successfully!\n");
                break;

            case 2: // question
                do 
				{//current security question and prompt for a new answer
                    printf("%s\nEnter new answer to your security question: ", user[user_index].security_questions);
                    fgets(new_security_answer, sizeof(new_security_answer), stdin);//read confirmation of new security answer
                    clean(new_security_answer);

                    printf("Re-enter new answer: ");
                    fgets(confirm_security_answer, sizeof(confirm_security_answer), stdin);
                    clean(confirm_security_answer);

                    if (strcmp(new_security_answer, confirm_security_answer) != 0) //if pass dont match
					{
                        printf("Security answers do not match. Try again.\n");
                    }
                } 
				while (strcmp(new_security_answer, confirm_security_answer) != 0);
				//update the user's security answer and save changes
                strcpy(user[user_index].security_answers, new_security_answer);
                save_accounts(user, user_counter);
                system("cls");
                printf("Security answer updated successfully!\n");
                break;

            case 3://back option
                printf("Returning to previous menu...\n");
                exit = 1; // exit loop
                break;

            default: //invalid input handling
                printf("Invalid choice. Please enter a number between 1 and 3.\n");
        }
    }
}

/*view_sent_messages displays all messages sent by the current user
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@return void (function does not return a value)
pre-condition: "messages.txt" must exist and be properly formatted; the user array is valid*/
void view_sent_messages(struct accounts user[], int user_index){
	FILE *file = fopen("messages.txt", "rt");
	struct message msg;
	int choice, found = 0, loop = 1;
	if (file == NULL) {
        printf("No messages found.\n");
        loop = 0;
    }
    
        while (loop && fscanf(file, "%50[^|]|%50[^|]|%99[^|]|%999[^\n]\n", 
                          msg.sender, msg.recipient, msg.subject, msg.body) != EOF) {
        if (strcmp(msg.sender, user[user_index].usernames) == 0) {//check if the current message was sent by the logged in user
            system("cls");
            printf("Sent to: %s \nSubject: %s\nMessage: %s\n", msg.recipient, msg.subject, msg.body);
            printf("-------------------------------\n");
            found = 1;

            printf("1. Next Message\n2. Back\nEnter choice: ");
            scanf("%d", &choice);
            fflush(stdin);
            
            if (choice == 2) {//go back
                loop = 0;
            }
        }
    }
    
    if (!found) {//no sent msgs
        printf("No more messages.\n");
    }
    fclose(file);
    system("cls");
}

/*modify_personal_connections allows the current user to manage their personal connections by adding, viewing, and removing connections, as well as viewing which users have added them
@param user - array of account structures containing user information and personal connections
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of users in the user array
@return void (function does not return a value)
pre-condition: the user array must be valid with at least user_counter elements; save_accounts() is implemented*/ 
void modify_personal_connections(struct accounts user[], int user_index, int user_counter) {
    int choice = 0, found, connection_index, exit = 0;
    char username[50];

    while (!exit) {
        system("cls");
        printf("\nPersonal Connections Menu:\n");
        printf("1. Add a personal connection\n");
        printf("2. View personal connections\n");
        printf("3. Remove a personal connection\n");
        printf("4. View user connections\n");
        printf("5. Back\n");

        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n');  // Clear invalid input
            choice = 0;
        }
        getchar();  // Consume newline character

        if (choice == 5) {
            exit = 1;
			system("cls"); // Exit the loop
        }

        if (choice == 1) {  // Add a personal connection
            printf("Enter username to add: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';

            found = 0;
            int i = 0;
            while (i < user_counter) {
                if (strcmp(user[i].usernames, username) == 0 && i != user_index) {
                    found = 1;
                    int j = 0, duplicate = 0;
                    while (user[user_index].personal_connections[j][0] != '\0') {
                        if (strcmp(user[user_index].personal_connections[j], username) == 0) {
                            duplicate = 1;
                        }
                        j++;
                    }
                    if (!duplicate) {
                        strcpy(user[user_index].personal_connections[j], username);
                        user[user_index].personal_connections[j + 1][0] = '\0';
                        printf("User '%s' added to personal connections.\n", username);
                    } else {
                        printf("User already in your personal connections.\n");
                    }
                }
                i++;
            }
            if (!found) {
                printf("User not found or cannot add yourself.\n");
            }

        } else if (choice == 2) {  // View personal connections
            printf("\nYour Personal Connections:\n");
            int j = 0;
            while (user[user_index].personal_connections[j][0] != '\0') {
                int k = 0;
                while (k < user_counter) {
                    if (strcmp(user[k].usernames, user[user_index].personal_connections[j]) == 0) {
                    	if(!(strcmp(user[k].usernames,"DELETED USER") == 0)){
                    	printf("- %s (@%s)\n", user[k].names, user[k].usernames);	
						}
                        
                    }
                    k++;
                }
                j++;
            }

        } else if (choice == 3) {  // Remove a personal connection
            printf("Enter username to remove: ");
            fgets(username,sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';

            connection_index = -1;
            int j = 0;
            while (user[user_index].personal_connections[j][0] != '\0') {
                if (strcmp(user[user_index].personal_connections[j], username) == 0) {
                    connection_index = j;
                }
                j++;
            }

            if (connection_index != -1) {
                while (user[user_index].personal_connections[connection_index][0] != '\0') {
                    strcpy(user[user_index].personal_connections[connection_index], user[user_index].personal_connections[connection_index + 1]);
                    connection_index++;
                }
                printf("User '%s' removed from personal connections.\n", username);
            } else {
                printf("User not found in personal connections.\n");
            }

        } else if (choice == 4) {  // View user connections
            printf("\nUsers who have added you as a connection:\n");
            int i = 0;
            while (i < user_counter) {
                if (i != user_index) {
                    int j = 0;
                    while (user[i].personal_connections[j][0] != '\0') {
                        if (strcmp(user[i].personal_connections[j], user[user_index].usernames) == 0) {
                            printf("- %s (@%s)\n", user[i].names, user[i].usernames);
                        }
                        j++;
                    }
                }
                i++;
            }

        }

        if (!exit) {
            printf("\n1. Back\n");
            while (getchar() != '\n');
        }
    }

    save_accounts(user, user_counter); // Save changes after exiting the menu
}

/*private_message sends a private message from the current user to a specified recipient
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of users in the user array
@param recipient - string specifying the recipient's username
@return void (function does not return a value)
pre-condition: the user array must be valid; "messages.txt" must be accessible for appending; function clean() is implemented*/
void private_message(struct accounts user[], int user_index, int user_counter, String recipient) {
    struct message msg;
    strcpy(msg.recipient,recipient);
			fflush(stdin);
            printf("Enter subject: ");
            fgets(msg.subject, sizeof(msg.subject), stdin); //reads subject from user input
            clean(msg.subject);
            
            msg.body[0] = '\0'; //msg body as empty string
            printf("Enter your message:\n");
            fgets(msg.body, sizeof(msg.body), stdin);//reads msg body from user input
            clean(msg.body);
			//sender username
            strcpy(msg.sender, user[user_index].usernames);

            FILE *file = fopen("messages.txt", "a");//save new changes in txt file
            

            fprintf(file, "%s|%s|%s|%s\n", msg.sender, msg.recipient, msg.subject, msg.body);
            

            fclose(file);
            system("cls");
            printf("Message sent successfully!\n");	
}


/*browse_users allows the current user to browse all registered users, view their profiles, add them as personal connections, or send private messages
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of users in the user array
@return void (function does not return a value)
pre-condition: the user array must be valid; functions save_accounts() and private_message() are implemented*/
void browse_users(struct accounts user[], int user_index, int user_counter) {
    int i, user_choice, selected_index;
    int choice, exit = 0, user_count = 1;
    system("cls");

    while (!exit) {
    system("cls");
        printf("=======================\n");
        printf("Browse Users\n");
        printf("1. View All Users\n");
        printf("2. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1://display all user with respective number
                printf("\nAll Users:\n");
                for (i = 0; i < user_counter; i++) {
                        printf("%d. %s (@%s)\n", user_count++, user[i].names, user[i].usernames);
                    
                }
                user_count = 1;//reset for the next iteration
                
                exit = 0;
                break;

             //user go back
            case 2:
                exit = 1;
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (exit == 1) {
            exit = 1;
        } else {
        
        	printf("\nEnter the number of the user to view their profile or '0' to go back: ");
            scanf("%d", &user_choice);	
		
            if (user_choice != 0 && (user_choice <= user_counter)) {// convert user # to an index
                selected_index = user_choice - 1;
                printf("\nProfile of %s (@%s):\n", user[selected_index].names, user[selected_index].usernames);
                printf("Description: %s\n", user[selected_index].description);
			
			while(user_choice -1 == user_index && choice != 3){
				printf("\n3. Back\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if(choice != 3){
                	printf("Please choose a valid input");
				}
			}
	
			if (user_choice -1!= user_index ){
				printf("\n1. Add as Personal Connection\n");
                printf("2. Send Private Message\n");
                printf("3. Back\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                  case 1: {
    				int k = 0;
    				int is_duplicate = 0;
    					//checks if the selected user is already  in the users connections

   					 while (user[user_index].personal_connections[k][0] != '\0' ){
        			is_duplicate += strcmp(user[user_index].personal_connections[k], user[selected_index].usernames) == 0;
        			k++;
    				}

    				if (is_duplicate == 0) {//add the selected user to the personal connections lists
       				 strcpy(user[user_index].personal_connections[k], user[selected_index].usernames);
        				user[user_index].personal_connections[k + 1][0] ='\0';

        			printf("User added to personal connections successfully.\n");
        			save_accounts(user, user_counter);
    				} else {
       				 printf("User is already in personal connections.\n");
   						 }
    				exit = 0;
    					break;
				 }
                    case 2:// send priv message to selected user
                        private_message(user, user_index, user_counter,user[selected_index].usernames);
                        exit = 0;
                        break;

                    case 3:
                        exit = 0;
                        break;

                    default:
                        printf("Invalid option.\n");
                }
				}
            }
            
        }
    }
    system("cls");
}

/*user_module provides a menu-driven interface for the current user to access various functionalities such as composing messages, viewing inbox, managing personal information, security settings, personal connections, and browsing other users
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of users in the system
@return void (function does not return a value)
pre-condition: the user array must be valid with at least user_counter elements; all functions invoked (e.g., compose_message, view_inbox) are implemented and operational*/ 
void user_module(struct accounts user[], int user_index, int user_counter) {
    int choice;
    int exit = 0;
    while (!exit) 
	{
        printf("\n===== USER MODULE =====\n");
        printf("1. Compose Message\n");
        printf("2. View Inbox\n");
        printf("3. View Sent Messages\n");
        printf("4. View Announcements\n");
        printf("5. Modify Personal Info\n");
        printf("6. Modify Account Security\n");
        printf("7. Modify Personal Connections\n");
        printf("8. Browse Users\n");
        printf("9. Logout\n");
        printf("=======================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        fflush(stdin);//remove extraneous input
        system("cls");

        switch (choice)
		 {
            case 1:
            	system("cls");
                compose_message(user, user_index, user_counter);
                break;
            case 2:
                view_inbox(user, user_index, user_counter);
                break; 
            case 3:
                view_sent_messages(user, user_index);
                break; 
            case 4:
                view_announcements(user,user_index, user_counter);
                break;
            case 5: 
                modify_personal_info(user, user_index, user_counter);
                break;
            case 6:
                modify_account_security(user, user_index, user_counter);
                break;
            case 7:
                modify_personal_connections(user, user_index, user_counter);
                break;
            case 8:
                browse_users(user, user_index, user_counter);
                break;
            case 9:
                exit = 1;
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 9.\n");
        }
    }
}



/*load_accounts loads account information from "accounts.txt" into the user array, and for each account, loads the associated personal connections from a separate file
@param user - array of account structures to be populated
@param user_counter - pointer to an integer that holds the number of loaded accounts
@return void (function does not return a value)
pre-condition: "accounts.txt" and "personal_connections_<username>.txt" files are formatted correctly; user array is allocated with space for at least MAX_USERS elements*/
void load_accounts(struct accounts user[], int *user_counter) {
    int exit_flag = 0;
    
    FILE *file = fopen("accounts.txt", "rt");
    if (file == NULL) {// if file is opened
        exit_flag = 1;
    }

    if (!exit_flag) {
        *user_counter = 0;
        while (!exit_flag && fscanf(file, "%50[^|]|%50[^|]|%20[^|]|%100[^|]|%100[^|]|%100[^|]|%100[^|]|%50[^\n]\n",
                                    user[*user_counter].usernames, user[*user_counter].names,
                                    user[*user_counter].passwords, user[*user_counter].security_questions,
                                    user[*user_counter].security_answers, user[*user_counter].description,
                                    user[*user_counter].lock, user[*user_counter].request) != EOF) {
            (*user_counter)++;
            if (*user_counter >= MAX_USERS) {
                exit_flag = 1;
            }
        }
        fclose(file);
    }

    FILE *file2 = NULL;
    if (!exit_flag) {
        file2 = fopen("personal_connections.txt", "rt");
        if (file2 == NULL) {
            exit_flag = 1;
        }
    }

    if (!exit_flag) {
        int index = 0;
        char line[500];

        while (!exit_flag && fgets(line, sizeof(line), file2) != NULL && index < *user_counter) {
            char *token = strtok(line, "|");
            int valid = (token != NULL);
            
            if (valid) {
                if (strcmp(token, user[index].usernames) == 0) {
                    int j = 0;
                    token = strtok(NULL, "|");
                    
                    while (token != NULL && j < 10) {
                        strcpy(user[index].personal_connections[j], token);
                        j++;
                        token = strtok(NULL, "|");
                    }
                    user[index].personal_connections[j][0] = '\0';  
                }
                index++;
            } else {
                exit_flag = 1;
            }
        }
        fclose(file2);
    }

    
}

/*create_new_acc creates a new account by gathering user input for name, username, password, and security question answer, then saves the account information
@param user - array of account structures to store account details
@param user_counter - pointer to an integer representing the current count of accounts
@return void (function does not return a value)
pre-condition: the user array must have sufficient space for new accounts; functions clean() and save_accounts() must be implemented*/
void create_new_acc(struct accounts user[], int *user_counter){
String password_check;
char rand_security_questions[15][101] = { // security questions to be asked
    "What is your favorite color?",
    "What is your favorite animal?",
    "What is your favorite number?",
    "What is your favorite movie?",
    "What is your favorite song?",
    "What is your favorite sport?",
    "What is your favorite hobby?",
    "What is your birth month?",
    "What is your lucky number?",
    "What is your favorite season?",
    "What is your favorite drink?",
    "What is your favorite snack?",
    "What is your childhood nickname?",
    "What is your dream job?",
    "What is your favorite holiday?"
};

	printf("Name: "); // name
	fgets(user[*user_counter].names,sizeof(user[*user_counter].names),stdin);
	clean(user[*user_counter].names);
	fflush(stdin);
	
	printf("Username: "); // username
	fgets(user[*user_counter].usernames,sizeof(user[*user_counter].usernames),stdin);
	clean(user[*user_counter].usernames);
	fflush(stdin);
		
	do {
		
	printf("Password: "); // password
	fgets(user[*user_counter].passwords,sizeof(user[*user_counter].passwords),stdin);
	clean(user[*user_counter].passwords);
	fflush(stdin);
	
	printf("Re-enter password: "); // re enter password
	fgets(password_check,sizeof(password_check),stdin);
	clean(password_check);
	fflush(stdin);	
	
	if (strcmp(user[*user_counter].passwords, password_check) != 0) {
        printf("Passwords do not match. Please enter again.\n");
    }
    
	} while (strcmp(user[*user_counter].passwords, password_check) != 0);
	//randomly select security questions from the predefined array	
	int security_index = rand() % 15;
	printf ("Security question: %s ", rand_security_questions[security_index]);
    fgets(user[*user_counter].security_answers, sizeof(user[*user_counter].security_answers), stdin);
    clean(user[*user_counter].security_answers);
	//store the questions into users account struct

	strcpy(user[*user_counter].security_questions,rand_security_questions[security_index]); // copies the security question to the security question struct user array
	fflush(stdin);
	
	system("cls");
	strcpy(user[*user_counter].description,"DEFAULT USER");
	strcpy(user[*user_counter].lock,"unlocked");
	strcpy(user[*user_counter].request,"no");
	printf("Account created! \n");
	(*user_counter)++; // user counter + 1
	save_accounts(user, *user_counter); // account gets saved
}

/* caesar_cipher encrypts an input string by shifting its alphabetic characters by a given value
@param input_text - pointer to the null-terminated string to be encrypted
@param shift_value - integer specifying the number of positions each letter is shifted
@param output_text - pointer to the character array where the encrypted string will be stored
@return void (function does not return a value)
pre-condition: input_text and output_text must be valid null-terminated strings; output_text must have enough space for the result*/
void caesar_cipher(char *input_text, int shift_value, char *output_text) {
    int index;
    for (index = 0; input_text[index] != '\0'; index++) {
        if (input_text[index] >= 'A' && input_text[index] <= 'Z') {
            output_text[index] = ((input_text[index] - 'A' + shift_value) % 26) + 'A';
        } else if (input_text[index] >= 'a' && input_text[index] <= 'z') {
            output_text[index] = ((input_text[index] - 'a' + shift_value) % 26) + 'a';
        } else {
            output_text[index] = input_text[index];
        }
    }
    output_text[index] = '\0';
}

/*modify_user allows the current user to update their account details, including name, username, description, and personal connections
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of users in the system
@return void (function does not return a value)
pre-condition: the user array must be valid; functions save_accounts() and Search() are implemented*/ 
void modify_user(struct accounts user[], int user_index, int user_counter) {
    int choice;
    String temp_username;
    do {//loop until user choose exit
    	system("cls");
        printf("\nModify User:\n");
        printf("1. Change Name\n");
        printf("2. Change Username\n");
        printf("3. Change Description\n");
        printf("4. Modify Personal Connections\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) { //change name
            printf("Enter new name: ");
            scanf("%s", user[user_index].names);//reads new name into users name field
        } else if (choice == 2) {//change username
            printf("Enter new username: ");
            scanf("%s", temp_username);
        
             updateUsernameInFiles(user[user_index].usernames,temp_username);
             strcpy(user[user_index].usernames,temp_username);//reads username into  a temp variable
             save_accounts(user, user_counter);
        } else if (choice == 3) {//change description
            printf("Enter new description: ");
            scanf("%s", user[user_index].description);//read new description into the user description field
        } else if (choice == 4) {
            printf("Modify Personal Connections:\n");
            printf("1. Add\n");
            printf("2. Remove\n");
            printf("Enter choice: ");
            int sub_choice;
            scanf("%d", &sub_choice);

            if (sub_choice == 1) {
                String new_connection;		
                printf("Enter new personal connection: ");
                scanf("%s", new_connection);
                if (Search(user, new_connection, user_counter) != -1) {
   				 int valid = 0;
    			for (int i = 0; i < 10 && !valid; i++) {
       			 if (user[user_index].personal_connections[i][0] == '\0') {
            	strcpy(user[user_index].personal_connections[i], new_connection);
           	 	valid = 1;
        		}
    		}
			} else {
                    printf("User not found.\n");
                }
            } else if (sub_choice == 2) {
                String remove_connection;
                printf("Enter personal connection to remove: ");
                scanf("%s", remove_connection);
                //int i = 0;
               	int valid = 0;
					for (int i = 0; i < 10 && !valid; i++) {
    				if (strcmp(user[user_index].personal_connections[i], remove_connection) == 0) {
        			user[user_index].personal_connections[i][0] = '\0';
        			valid = 1;
   					 }
					}

                }
            }
        }while (choice != 5);
    system("cls");
    save_accounts(user, user_counter);
    } 


/*refresh_password resets the current user's password to a default value and updates their request status, then saves the updated account information
@param user - array of account structures containing user information
@param user_index - index of the currently logged in user in the user array
@param user_counter - total number of users in the system
@return void (function does not return a value)
pre-condition: the user array must be valid with at least user_counter elements; save_accounts() is implemented*/
void refresh_password(struct accounts user[], int user_index, int user_counter) {
	system("cls");
    printf("Password has been reset to 'default'.\n");
    strcpy(user[user_index].passwords, "default");
    strcpy(user[user_index].request,"no");
    save_accounts(user, user_counter);
}

/*delete_user marks the specified user as deleted by setting their name and username to "DELETED USER", then saves the updated account data
@param user - array of account structures containing user information
@param index - index of the user to be marked as deleted in the user array
@param user_counter - total number of user accounts in the system
@return void (function does not return a value)
pre-condition: the user array must be valid; save_accounts() is implemented to persist changes*/ 
void delete_user(struct accounts user[], int index, int user_counter) {
	String temp_username;
	strcpy(temp_username,user[index].usernames);
    strcpy(user[index].names,"DELETED USER");
    strcpy(user[index].usernames,"DELETED USER");
    save_accounts(user, user_counter);
    updateUsernameInFiles(temp_username, "DELETED USER");
     
}

/*view_users displays detailed information for each user account, allowing navigation through the list and various account management actions
@param user - array of account structures containing user information
@param user_counter - pointer to an integer representing the total number of user accounts
@return void (function does not return a value)
pre-condition: the user array must be valid with at least *user_counter accounts; functions caesar_cipher(), modify_user(), refresh_password(), and delete_user() are implemented*/ 
void view_users(struct accounts user[], int *user_counter) {
    system("cls");
    int index = 0;
    int choice;
    int exit = 0;
    char encrypted_password[50];
    do {
        if (*user_counter == 0) {
            printf("No users available.\n");
            exit = 1;
        }
        
        if (!exit) {
        	system("cls");
            printf("User %d/%d:\n", index + 1, *user_counter);
            printf("Name: %s (@%s)\n", user[index].names, user[index].usernames);
            caesar_cipher(user[index].passwords, 3, encrypted_password);
            printf("Password (Encrypted): %s\n", encrypted_password);
            printf("Description: %s\n", user[index].description);
            printf("Account is %s\n", user[index].lock);
            printf("Account requested a password reset: %s\n", user[index].request);
            printf("Personal Connections: ");
            if (user[index].personal_connections[0][0] == '\0') {
                printf("None\n");
            } else {
                for (int j = 0; user[index].personal_connections[j][0] != '\0'; j++) {
                	if(!(strcmp(user[index].personal_connections[j],"DELETED USER")==0))
                    printf("%s -", user[index].personal_connections[j]);
                }
                printf("\n");
            }

            printf("\nOptions:\n");
            printf("1. Previous\n");
            printf("2. Next\n");
            printf("3. Modify User\n");
            printf("4. Refresh Password\n");
            printf("5. Delete User\n");
            printf("6. Unlock account\n");
            printf("7. Exit\n");
            printf("Enter choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    if (index > 0) {
                        index--;
                    }
                    break;
                case 2:
                    if (index < *user_counter - 1) {
                        index++;
                    }
                    break;
                case 3:
                    modify_user(user,index, *user_counter);
                    break;
                case 4:
                    refresh_password(user,index, *user_counter);
                    break;
                case 5:
                    delete_user(user, index, *user_counter);
                    break;
                case 6:
                    strcpy(user[index].lock, "unlocked");
                    break;
                case 7:
                	exit = 1;
                    break;
            }
        }
    } while (!exit);
}

/*load_admin_password reads the administrator password from "admin_pass.txt" and stores it in the provided buffer
@param admin_password - a character array where the admin password will be stored
@return void (function does not return a value)
pre-condition: "admin_pass.txt" must exist and be accessible; admin_password must be a valid writable buffer*/ 
void load_admin_password(String admin_password) {
    FILE *file = fopen("admin_pass.txt", "rt");
        fgets(admin_password, 51, file);
        clean(admin_password);
        fclose(file);
    
}

/*update_admin_password updates the administrator password by writing a new password to "admin_pass.txt", then reloads the updated password into the provided buffer
@param admin_password - a character array where the admin password will be stored after update
@param new_password - a string containing the new administrator password
@param exit_flag - pointer to an integer used to indicate if an error occurred during the update (set to 1 on error)
@return void (function does not return a value)
pre-condition: "admin_pass.txt" must be writable; admin_password must be a valid writable buffer; function load_admin_password() is implemented*/
void update_admin_password(String admin_password,String new_password, int *exit_flag) {
    FILE *file = fopen("admin_pass.txt", "wt");
    if (file == NULL) {
        printf("Error: Unable to update password file.\n");
        *exit_flag = 1; 
    } else {
        fprintf(file, "%s", new_password);
        fclose(file);
    }
    load_admin_password(admin_password);
}

/*change_admin_password allows the administrator to change the admin password by verifying the current password and, if valid, prompting for a new password and confirmation before updating it
@param admin_password - a character array containing the current administrator password
@return void (function does not return a value)
pre-condition: admin_password must be a valid null-terminated string; functions clean() and update_admin_password() are implemented; input is read from stdin*/
void change_admin_password(String admin_password) {
    String temp_password, new_password, confirm_password;
    int valid = 0, exit_flag = 0;


    if (!exit_flag) {
        printf("Enter current admin password: ");
        fgets(temp_password, sizeof(temp_password), stdin);
        clean(temp_password);

        if (strcmp(temp_password, admin_password) == 0) {
            valid = 1;
        } else {
            printf("Incorrect password. Password change failed.\n");
            exit_flag = 1;
        }
    }

    while (valid && !exit_flag) {
        printf("Enter new admin password: ");
        fgets(new_password, sizeof(new_password), stdin);
        clean(new_password);

        printf("Confirm new password: ");
        fgets(confirm_password, sizeof(confirm_password), stdin);
        clean(confirm_password);

        if (strcmp(new_password, confirm_password) == 0) {
            update_admin_password(admin_password,new_password, &exit_flag);
            if (!exit_flag) {
                printf("Admin password updated successfully!\n");
            }
            valid = 0;
        } else {
            printf("Passwords do not match. Try again.\n");
        }
    }
}

/*admin_users_module provides a menu interface for the administrator to manage user accounts, including viewing user details and performing actions like modifying, refreshing passwords, or deleting users
@param user - array of account structures containing user information
@param user_counter - pointer to an integer representing the total number of users in the system
@return void (function does not return a value)
pre-condition: the user array must be valid with at least *user_counter accounts; function view_users() is implemented*/
void admin_users_module(struct accounts user[], int *user_counter){
	int exit = 0, choice;
	while (!exit){
		system("cls");
		printf("\n== ADMINISTRATOR USER MODULE ===\n");
        printf("1. View Users (Modify User, Refresh Password, Delete User)\n");
        printf("2. Back \n");
        printf("===============================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        fflush(stdin);
        system("cls");
	
	
	switch (choice) {
            case 1:
               view_users(user , user_counter);
                break;
            case 2:
            	exit = 1;
            	break;
            default:
            	printf("Invalid choice. Please try again.\n"); // invalid
        }
        
  }
        
        
    }
    
    
/*view_messages displays messages from "messages.txt" by printing sender, recipient(s), subject, and message body.
@param user - array of account structures (not used directly here but provided for consistency)
@param user_counter - total number of users in the system (not directly used here)
@return void (function does not return a value)
pre-condition: "messages.txt" exists and is formatted correctly*/   
void view_messages(struct accounts user[], int user_counter) 
{
    FILE *file = fopen("messages.txt", "rt");
    struct message msg;
    int found = 0;
    if (file == NULL) 
	{
        printf("No messages found.\n");
        return;
    }
    //loop to read each msgs in files
    while (fscanf(file, "%50[^|]|%50[^|]|%50[^|]|%1000[^\n]\n", msg.sender, msg.recipient, msg.subject, msg.body) != EOF) 
	{
        //int sender_index;
        //int i = 0;

        printf("Name of the Sender: %s\n", msg.sender);
        //check if sent to all
        if (strcmp(msg.recipient, "all") == 0)
		{
            printf("Recipient: All\n");
        } 
			else 
		{
            printf("Recipient: %s\n", msg.recipient);
            FILE *temp_file = fopen("messages.txt", "rt"); //search for other recipient same msg
            struct message temp_msg;
            int first = 1;
            
            printf("Other Recipients: ");
            while (fscanf(temp_file, "%50[^|]|%50[^|]|%50[^|]|%1000[^\n]\n", temp_msg.sender, temp_msg.recipient, temp_msg.subject, temp_msg.body) != EOF) 
			{//loop thru file with msg same subject
                if (strcmp(msg.subject, temp_msg.subject) == 0 && strcmp(msg.body, temp_msg.body) == 0 && strcmp(temp_msg.sender, msg.sender) == 0 && strcmp(temp_msg.recipient, msg.recipient) != 0) 
				{//comma separator
                    if (!first) printf(", ");
                    printf("%s", temp_msg.recipient);
                    first = 0; //additional recipients
                }
            }
            fclose(temp_file);
            if (first) printf("None");
            printf("\n");
        }
		//diplays msgs
        printf("Subject: %s\nMessage: %s\n", msg.subject, msg.body);
        printf("----------------------------------------\n");
        found = 1; //message if found
    }
    
    fclose(file);
    if (!found) 
	{
        printf("No messages found.\n");
    }
}


/*view_messages_by_filter displays messages from "messages.txt" filtered by sender or recipient based on user input
@param none
@return void (function does not return a value)
pre-condition: "messages.txt" exists and is formatted correctly; input is read from stdin; function clean() is implemented*/
void view_messages_by_filter() 
{
    FILE *file = fopen("messages.txt", "rt");
    struct message msg;
    String filter;
    int choice;
	int found = 0, exit = 0;
    
    if (file == NULL) 
    {
        printf("No messages found.\n");
    } 
    else 
    {
        while (!exit) 
        {
            printf("Filter by:\n1. Sender\n2. Recipient\n3. Go Back\nEnter choice: ");
            scanf("%d", &choice);
            fflush(stdin);

            if (choice < 1 || choice > 3) 
            {
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
                printf("----------------------------------------\n");
                //system("cls"); tanginaaa pangit
            } 
            else if (choice == 3) 
            {
                system("cls");
                exit = 1;
            } 
            else 
            {
                printf("Enter name or username: ");
                fgets(filter, sizeof(String), stdin);
                clean(filter);
                system("cls");
                found = 0;
                
                while (fscanf(file, "%50[^|]|%50[^|]|%50[^|]|%1000[^\n]\n", msg.sender, msg.recipient, msg.subject, msg.body) != EOF) 
                {
                    if ((choice == 1 && strcmp(msg.sender, filter) == 0) || (choice == 2 && strcmp(msg.recipient, filter) == 0)) 
                    {
                        printf("From: %s\nTo: %s\nSubject: %s\nMessage: %s\n", msg.sender, msg.recipient, msg.subject, msg.body);
                        printf("----------------------------------------\n");
                        found = 1;
                    }
                }
                
                if (!found) 
                {
                    printf("No messages found for the specified filter.\n");
                }
                fclose(file);
                file = fopen("messages.txt", "rt"); 
            }
        }
    }
}


/*delete_message removes messages from "messages.txt" that match a user-specified subject and body
@param none
@return void (function does not return a value)
pre-condition: "messages.txt" exists and is formatted correctly; MAX_MESSAGES is defined; user input is valid*/
void delete_message() {
    FILE *file;
    struct message messages[MAX_MESSAGES];
    int message_count = 0, i, deleted = 0, exit_flag = 0;
    String subject, body;

    file = fopen("messages.txt", "rt");
    if (file == NULL) {
        printf("No messages to delete.\n");
        exit_flag = 1; 
    }

    if (!exit_flag) {
    
        while (message_count < MAX_MESSAGES && fscanf(file, "%50[^|]|%50[^|]|%50[^|]|%1000[^\n]\n",
                messages[message_count].sender,
                messages[message_count].recipient,
                messages[message_count].subject,
                messages[message_count].body) != EOF) {
            message_count++;//count for each msgs read
        }
        fclose(file);

        printf("Enter subject of message to delete: ");
        fgets(subject, sizeof(subject), stdin);
        clean(subject);

        printf("Enter full message body to match: ");
        fgets(body, sizeof(body), stdin);
        clean(body);

        file = fopen("messages.txt", "wt");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            exit_flag = 1;
        }
    }

    if (!exit_flag) {
        for (i = 0; i < message_count; i++) {//go thru stores msgs
            if (!(strcmp(messages[i].subject, subject) == 0 && strcmp(messages[i].body, body) == 0)) {
                fprintf(file, "%s|%s|%s|%s\n",//write msgs back to file if does not fit the delete criteria
                        messages[i].sender,
                        messages[i].recipient,
                        messages[i].subject,
                        messages[i].body);
            } else {
                deleted = 1;
            }
        }
        fclose(file);

        if (deleted) {
            printf("Message(s) deleted successfully.\n");
        } else {
            printf("Message(s) not found.\n");
        }
    }
}

/*messages_module provides a menu interface for handling message-related operations, including viewing all messages, filtering messages, and deleting messages
@param user - array of account structures containing user information (used by view_messages)
@param user_counter - total number of users in the system (used by view_messages)
@return void (function does not return a value)
pre-condition: the user array must be valid; functions view_messages(), view_messages_by_filter(), and delete_message() are implemented*/
void messages_module(struct accounts user[], int user_counter) {
    int choice;
    int exit = 0;
    while (!exit) {
        printf("\n===== MESSAGES MODULE =====\n");
        printf("1. View Messages\n");
        printf("2. View Messages by Filter\n");
        printf("3. Delete Message\n");
        printf("4. Back\n");
        printf("===========================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        fflush(stdin);
        system("cls");
        
        switch (choice) {
            case 1:
                view_messages(user, user_counter);
                break;
            case 2:
                view_messages_by_filter();
                break;
            case 3:
                delete_message();
                break;
            case 4:
                exit = 1;
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
    }
}

/*administrator_module authenticates the administrator and, upon successful verification, provides a menu interface for accessing administrative functions such as managing users, messages, and changing the admin password
@param user - array of account structures containing user information
@param user_counter - pointer to an integer representing the total number of user accounts
@param admin_password - a string containing the current administrator password
@return void (function does not return a value)
pre-condition: the user array must be valid; admin_password is a valid null-terminated string; functions clean(), admin_users_module(), messages_module(), and change_admin_password() are implemented*/
void administrator_module(struct accounts user[], int *user_counter, String admin_password) 
{
    String entered_password;
    int choice;
    int granted = 0;
    
    do { 
    printf("Enter Administrator Password: ");
    fgets(entered_password, sizeof(entered_password), stdin);
    clean(entered_password);	
    fflush(stdin);
    if (strcmp(entered_password, admin_password) != 0){
    	printf("Incorrect password. Access denied.\n");
	}
	} while (strcmp(entered_password, admin_password) != 0);
	
    granted = 1;
   system("cls");
    
    
    while (granted) 
	{
		
        printf("\n===== ADMINISTRATOR MODULE =====\n");
        printf("1. Users Module\n");
        printf("2. Messages Module\n");
        printf("3. Change Admin Password\n");
        printf("4. Back to Main Menu\n");
        printf("===============================\n");
        printf("Enter choice: ");
        
        scanf("%d", &choice);
        fflush(stdin);
        system("cls");
        
        switch (choice)
		{
            case 1:
                admin_users_module(user, user_counter);
                break;
            case 2:
                messages_module(user, *user_counter);
                break;
            case 3:
                change_admin_password(admin_password);
                break;
            case 4:
                granted = 0;
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
        
    }
}

/*login authenticates a user by prompting for username and password, checks account status, and locks the account after three failed attempts.
@param user - array of account structures containing user information
@param user_counter - total number of users in the system
@return void (function does not return a value)
pre-condition: the user array is valid; functions clean(), Search(), save_accounts(), and user_module() are implemented; input is read from stdin*/
void login(struct accounts user[], int user_counter) {
    char temp_username[51], temp_password[51];
    int user_index, attempts = 0, exit_flag = 0;

    do { // username input
        printf("Username: ");
        fgets(temp_username, sizeof(temp_username), stdin);
        clean(temp_username);

        user_index = Search(user, temp_username, user_counter); // search for user index
        if (user_index == -1 || strcmp(user[user_index].usernames, "USER DELETED") == 0) {
            printf("Username not found. Try again.\n");
        }
    } while (user_index == -1);

    // Check if the account is already locked or deleted
    if (strcmp(user[user_index].passwords, "default") == 0 ||
        strcmp(user[user_index].lock, "locked") == 0) {
        system("cls");
        printf("Account password must be reset, account is locked, or account is deleted.\n");
    } else {
        do { // password input
            printf("Password: ");
            fgets(temp_password, sizeof(temp_password), stdin);
            clean(temp_password);

            if (strcmp(user[user_index].passwords, temp_password) != 0) {
                attempts++;
                printf("Incorrect password. Attempts left: %d\n", 3 - attempts);
                
                if (attempts >= 3) {
                    strcpy(user[user_index].lock, "locked");
					save_accounts(user, user_counter);
                    printf("Account locked due to multiple failed login attempts.\n");
                    exit_flag = 1;
                }
            } else {
                exit_flag = 1;
            }
        } while (!exit_flag);

        if (attempts < 3) {
            printf("Login successful!\n");
            system("cls");
            user_module(user, user_index, user_counter);
        }
    }
}

/*forgot_password verifies a user's identity using their security question and, if verified, allows them to reset their password
@param user - array of account structures containing user information
@param user_counter - total number of users in the system
@return void (function does not return a value)
pre-condition: the user array is valid; functions clean(), Search(), and save_accounts() are implemented; input is read from stdin*/
void forgot_password(struct accounts user[], int user_counter) 
{
    String temp_username, temp_answer, new_password, confirm_password;
    int user_index;
    int entry = 1;

   do {
    printf("Enter your username: ");
    fgets(temp_username, sizeof(temp_username), stdin);
    clean(temp_username);
	user_index = Search(user, temp_username, user_counter);
	if (user_index == -1) 
    {
        printf("Username not found Please try again.\n");
    }
    }while (user_index == -1);
    
    do{
    printf("Security Question: %s\n", user[user_index].security_questions);
        printf("Answer: ");
        fgets(temp_answer, sizeof(temp_answer), stdin);
        clean(temp_answer);

        if (strcmp(user[user_index].security_answers, temp_answer) != 0) {
            printf("Incorrect answer. Request failed.\n");
        }	
	}while(strcmp(user[user_index].security_answers, temp_answer) != 0);
        
    strcpy(user[user_index].request,"yes");    

    if (entry && strcmp(user[user_index].passwords,"default") == 0 ) {
        do 
        {
            printf("Enter new password: ");
            fgets(new_password, sizeof(new_password), stdin);
            clean(new_password);

            printf("Confirm new password: ");
            fgets(confirm_password, sizeof(confirm_password), stdin);
            clean(confirm_password);

            if (strcmp(new_password, confirm_password) != 0) 
            {
                printf("Passwords do not match. Try again.\n");
            }
        } 
        while (strcmp(new_password, confirm_password) != 0);

        strcpy(user[user_index].passwords, new_password);
        strcpy(user[user_index].request,"no"); 
        printf("Password reset successful!\n");
        printf("==============================\n");
    }
     save_accounts(user, user_counter);
     system("cls");
    if (strcmp(user[user_index].request,"yes") ==0){
    	printf("Password change reqeusted!\n");
	}
}

/*login_page displays the login menu and allows users to log in, create a new account, request a forgotten password, or return to the main menu
@param user - array of account structures containing user information
@param user_counter - total number of users in the system
@return void (function does not return a value)
pre-condition: the user array is valid; functions login(), create_new_acc(), and forgot_password() are implemented; MAX_USERS is defined*/
void login_page(struct accounts user[], int user_counter) {
    int choice;
	int exit = 0;
    while (!exit) { 
        printf("\n===== LOGIN MENU =====\n");
        printf("1. Log in\n");
        printf("2. Create a new account\n");
        printf("3. Request a forgotten password\n");
        printf("4. Back to Main Menu\n");
        printf("======================\n");
        printf("Enter choice: ");
        
        scanf("%d", &choice);
        fflush(stdin);
		system("cls");
		
        switch (choice) {
            case 1:
                if (user_counter == 0) {
                    printf("No users created yet!\n"); // no users created yet
                } else {
                    login(user, user_counter); // logins the user
                }
                break;

            case 2:
                if (user_counter < MAX_USERS) {
                    create_new_acc(user, &user_counter); // create new acc
                } else {
                    printf("Maximum user limit reached!\n\n"); // max reached
                }
                break;

            case 3:
                forgot_password(user, user_counter);
                break;

            case 4:
                printf("Returning to Main Menu...\n"); // return to main menu
                exit =1;
                break;

            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n"); // invalid
        }
    }
}





int main ()
{
String admin_password; //dustine cute
load_admin_password(admin_password);
struct accounts user[MAX_USERS];
int user_counter = 0;
int choice;
srand(time(NULL));
load_accounts(user, &user_counter);
int exit = 0;
 while (!exit) {
        choice = main_menu();
		system("cls");
        switch (choice) {
            case 1:
                login_page(user, user_counter); // redirects to login poge
                break;
            case 2:
            	system("cls");
            	administrator_module(user, &user_counter, admin_password); // administrator module 
                break;
            case 3:
                printf("Exiting program...\n"); // exit program
                exit = 1;
                break;
            default:
            	system("cls");
            	printf("Invalid choice. Please try again.\n"); // invalid
        }
    }
    return 0;
}





