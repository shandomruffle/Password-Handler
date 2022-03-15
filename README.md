# Password-Handler

App to manage and store your accounts secured with a password.

## Interface

### Login window

Come up with a master password (the longer it is the harder it is to be hacked). It is used to protect your account pack from viewing outside this app.

You can use multiple logins for multiple account packs.

![image](https://user-images.githubusercontent.com/34777382/158470159-f4d217c4-ea1f-4a47-9612-763b6659bbe0.png)

### Main window

Enter new account info and press `Add` button to insert it into the table.

Select a cell and press `Alt` key or double-click to edit text, Enter to save changes.

Press `Delete` or `Backspace` key from a cell to erase the whole row (and the corresponding account).

`Save` button to save the whole table into a file (by default into `packs/<login>.pw-pack`).

`Save as` button to choose a different file for saving. The app will remember your choice.

`Open` button to import a table from a different file. That file must have been encrypted with the same master password.

![image](https://user-images.githubusercontent.com/34777382/158470217-d0b31ae5-387b-4a28-9de1-d33e4a1f9894.png)
