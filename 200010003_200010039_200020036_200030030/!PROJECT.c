#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#define sleep(n) Sleep(1000 * (n))
#else
#include <unistd.h>
#endif  // for Sleep()

void seller_login();                                           //declaration of functions prior to using them, like a summary
void seller_addaccount();
void seller_choice();
void seller_itemadd();
void seller_displayall();                                      //definitely way easier to manage in a DBMS

void customer_choice();
void customer_viewall();
void customer_viewcat();
void customer_addtocart();
void customer_checkcart();
void customer_checkout();

int logged_in;                                                 // global variable, can be assigned/accessed anywhere

struct item                                                    // template for the item records
{
    int item_id;
    char item_name[30];
    float price;
    int stock_qty;
    char features[25];
    int item_seller_id;
}s;

int main()
{
    int ch;
    FILE *cart; //to clear the cart in a new session

    cart=fopen("cart.txt","w");
    fclose(cart);

    while(1)
    {
        system("cls");
        //Initial front-end display of the site
        printf("\t\t\t**********WELCOME TO OUR E-COMMERCE WEBSITE**********");

        //We want a time delay. We could have a loop do 47765657 iterations but it's
        // much more responsible to use Sleep() so that the CPU doesn't have too much work
        sleep(2);

        printf("\n1:Existing Seller");
        printf("\n2:Customer");
        printf("\n3:New Seller");
        printf("\n4:Exit");
        printf("\n\tEnter Your Choice: ");
        scanf("%d", &ch);
        switch(ch)
        {
            case 1:
                seller_login();
                seller_choice();
                break;
            case 2:
                customer_choice();
                break;
            case 3:
                seller_addaccount();
                printf("\nUse these credentials to log in as existing seller.\nEnter any character.\n");
                getch();
                continue;                                      //once registered, log in as an existing seller
            case 4:
                exit(1);
            default:
                printf("\n\tInvalid Option\n");
                break;
        }
    }
    getch();
    return 0;
}

void seller_addaccount()                                       //function to register as a new seller
{
    int new_seller_id, id, status;
    char new_seller_username[20], user[20];
    FILE *sl;
    system("cls");

    printf("Welcome ! Thank you for choosing our site! ");
    sl=fopen("seller_list.txt","a+");
    do{
            status=0;
            printf("\nEnter a username: ");
            fflush(stdin);                                     //to clear extra whitespace characters
            fgets(new_seller_username, 19, stdin);
            new_seller_username[strlen(new_seller_username)-1]='\0';
            printf("\nEnter an ID of 6 digits: ");
            scanf("%d", &new_seller_id);
            if(new_seller_id > 999999 || new_seller_id < 100000) //ensures it's a 6-digit number
            {
                printf("\ninvalid id!\n");
                continue;
            }
            fseek(sl, 0, SEEK_SET);                            //to ensure it begins reading from start of file
            while(fscanf(sl, "%20s %6d\n", user, &id) != EOF)  //traverses the lines of the file
            {
                if(strcmp(user, new_seller_username) == 0)     //to ensure unique username
                    {
                        printf("\nUsername Taken. Try again");
                        status = 1;
                        break;
                    }
                if(id == new_seller_id)                        //to ensure unique seller ID
                    {
                        printf("\nID Taken. Try again");
                        status = 1;
                        break;
                    }
            }
    }while(status == 1);                                       //keeps looping until valid input
    fprintf(sl, "%20s %6d\n", new_seller_username, new_seller_id);
    fclose(sl);
}

void seller_login()
{
    int input_id, id, ii;
    char input_username[20], user[20];
    FILE *sl;
    system("cls");

    printf("\t\t\t~~~~~~~~~~SELLER LOGIN PORTAL~~~~~~~~~~");
    sl = fopen("seller_list.txt","r");                         //r mode because we only read the file.
    while(1)
        {
            fseek(sl, 0, SEEK_SET);
            printf("\nEnter username: ");
            fflush(stdin);                                     //to clear extra whitespace characters
            fgets(input_username, 19, stdin);
            input_username[strlen(input_username)-1] = '\0';   //to make the final \n into a null character
            printf("\nEnter your ID: ");
            scanf("%d", &input_id);
            while(fscanf(sl, "%20s %6d\n", user, &id) != EOF)  //traverses the lines of the file
            {
                ii = strcmp(user, input_username);
                if(ii == 0 && (id == input_id))
                    {
                        printf("\nLOGGED IN.\n\n");
                        fflush(stdin);
                        logged_in = id;                        // assign the id to the global variable for easy access
                        return;
                    }
            }
            printf("\nInvalid ID or Username.");
        }
    fclose(sl);
}

void seller_choice()
{
    int ch;
    system("cls");

    while(1)
    {
        printf("\t\t\t**********Seller Dashboard**********");  //Seller is given the following options
        printf("\n1:Add items to your inventory");
        printf("\n2:Display all items");
        printf("\n3:Exit");
        printf("\n\tEnter Your Choice: ");
        scanf("%d", &ch);
        switch(ch)
        {
            case 1:
                seller_itemadd();
                break;
            case 2:
                seller_displayall();
                break;
            case 3:
                exit(1);
            default:
                printf("\n\tInvalid Option\n");
                break;
        }
    }
    getch();
}

void seller_itemadd()
{
    int i, cat, status, n = 0;
    char filename[25];

    //category block
    do{
            status = 1;
            printf("\nCategories:\n1:Vegetables\n2:Dry Fruits\n");
            printf("3:Fruits\n4:Snacks\n5:Refreshments\n6:Return to dashboard\n\nMake a choice: ");
            scanf("%d", &cat);
            switch(cat)
            {
                case 1:
                    strcpy(filename, "vegetables.txt");
                    status=0;
                    break;
                case 2:
                    strcpy(filename, "dry-fruits.txt");
                    status=0;
                    break;
                case 3:
                    strcpy(filename, "fruits.txt");
                    status=0;
                    break;
                case 4:
                    strcpy(filename, "snacks.txt");
                    status=0;
                    break;
                case 5:
                    strcpy(filename, "refreshments.txt");
                    status=0;
                    break;
                case 6:
                    return;
                default:
                    printf("\ninvalid category\n");
            }
    }while(status == 1);

    FILE *fp;
    fp = fopen(filename, "a+");                                //appending to the file of the category

    printf("\n\tHow Many Items Do You Want to Enter:");        // since seller may be adding multiple items to the category
    scanf("%d",&n);

    for(i=0; i<n; i++)
    {
        fflush(stdin);
        int new_item_id, t = 0;
        struct item s;
        while(t == 0)
        {
            t = 1;
            printf("\nEnter Item ID: ");
            scanf("%d", &new_item_id);
            fflush(stdin);
            fseek(fp, 0, SEEK_SET);
            while(fscanf(fp, "%6d %20s %f %4d %20s %6d\n", &s.item_id, s.item_name, &s.price, &s.stock_qty, s.features, &s.item_seller_id) != EOF) //traverses the lines of the file
            {
                if(s.item_id == new_item_id)                   //to ensure unique item ID
                    {
                        printf("\nItem ID taken.");
                        t = 0;
                    }
            }
        }

        s.item_id = new_item_id;

        printf("Enter Item Name: ");                           //take in all inputs of item info
        fflush(stdin);
        fgets(s.item_name, 29, stdin);
        s.item_name[strlen(s.item_name)-1] = '\0';             //IMP: don't put spaces in between strings. hard for file reading

        printf("Enter Price: ");
        scanf("%f",&s.price);

        printf("Enter quantity in stock: ");
        scanf("%d",&s.stock_qty);

        printf("Enter other features: ");
        fflush(stdin);
        fgets(s.features, 24, stdin);
        s.features[strlen(s.features)-1]='\0';

        s.item_seller_id = logged_in;

        printf("\n*****************\n");

        fseek(fp, 0, SEEK_END);
        fprintf(fp, "%6d %20s %6.2f %4d %20s %6d\n", s.item_id, s.item_name, s.price, s.stock_qty, s.features, s.item_seller_id);
    }
    fclose(fp);
}

void seller_displayall()
{
    int i, seller_id;
    char cat_array[5][20]={"vegetables.txt\0","dry-fruits.txt\0","fruits.txt\0","snacks.txt\0","refreshments.txt\0"};
    struct item s;
    system("cls");

    seller_id = logged_in;                                     //Take in the value from the global variable

    printf("\t\t\t**********SELLER INVENTORY**********\n");
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
    printf("\t|%-6s|%-20s|%-9s|%-11s|%-20s|%-9s|\n","ItemID","Item Name","Price","Qty in stock","Features", "Seller ID");
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");

    FILE *fp;
    for(i=0; i<5; i++)
    {
        fp = fopen(cat_array[i], "r");
        if(fp==NULL)
        {
            continue;
        }
        while(fscanf(fp, "%6d %20s %f %4d %20s %6d\n", &s.item_id, s.item_name, &s.price, &s.stock_qty, s.features, &s.item_seller_id) != EOF)
        {
            if(seller_id == s.item_seller_id)                  //Display only records belonging to the logged-in seller
            {
                printf("\t|%6d|%-20s|%9.2f|   %9d|%-20s|%6d   |\n", s.item_id, s.item_name, s.price, s.stock_qty, s.features, s.item_seller_id);
            }
        }
        fclose(fp);
    }
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
}

void customer_choice()
{
    int ch;
    system("cls");

    while(1)
    {
        printf("\t\t*******WELCOME DEAR CUSTOMER*******");     //Customer is given the following options
        printf("\n1:Display all items.");
        printf("\n2:Display a category.");
        printf("\n3:Add item to cart.");
        printf("\n4:Check Cart");
        printf("\n5:Exit");
        printf("\n\tEnter Your Choice: ");
        scanf("%d", &ch);
        switch(ch)
        {
            case 1:
                customer_viewall();
                break;
            case 2:
                customer_viewcat();
                break;
            case 3:
                customer_addtocart();
                break;
            case 4:
                customer_checkcart();
                break;
            case 5:
                exit(1);
            default:
                printf("\n\tInvalid Option\n");
                break;
        }
    }
    getch();
}

void customer_viewcat()
{
    int status, cat;
    char filename[25];
    system("cls");

    //category block, choose the category file
    do{
            status = 1;
            printf("\nCategories:\n1:Vegetables\n2:Dry Fruits\n");
            printf("3:Fruits\n4:Snacks\n5:Refreshments\n6:Return to dashboard\n\nMake a choice: ");
            scanf("%d", &cat);
            switch(cat)
            {
                case 1:
                    strcpy(filename, "vegetables.txt");
                    status=0;
                    break;
                case 2:
                    strcpy(filename, "dry-fruits.txt");
                    status=0;
                    break;
                case 3:
                    strcpy(filename, "fruits.txt");
                    status=0;
                    break;
                case 4:
                    strcpy(filename, "snacks.txt");
                    status=0;
                    break;
                case 5:
                    strcpy(filename, "refreshments.txt");
                    status=0;
                    break;
                case 6:
                    return;
                default:
                    printf("\ninvalid category\n");
            }
    }while(status == 1);

    FILE *fp;
    fp = fopen(filename, "r");
    if(fp==NULL)
        {
            printf("\nSome problem occured. Please try again.\nTechnical team has been notified of the issue and will try to fix it soon.");
            exit(1);
        }
    filename[strlen(filename)-4]='\0';                         //remove .txt, use the rest in the heading.

    printf("\t\t\t**********STORE: CATEGORY-%s**********\n", filename);
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
    printf("\t|%-6s|%-20s|%-9s|%-11s|%-20s|%-6s|\n","ItemID","Item Name","Price","Qty in stock","Features", "Seller ID");
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
    while(fscanf(fp, "%6d %20s %f %4d %20s %6d\n", &s.item_id, s.item_name, &s.price, &s.stock_qty, s.features, &s.item_seller_id) != EOF)
        {
            if(s.stock_qty>0)
            {
                printf("\t|%6d|%-20s|%9.2f|   %9d|%-20s|%6d   |\n", s.item_id, s.item_name, s.price, s.stock_qty, s.features, s.item_seller_id);
            }
        }
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n\n");

    fclose(fp);
}

void customer_viewall()
{
    int i;
    char display_array[5][20]={"vegetables.txt\0","dry-fruits.txt\0","fruits.txt\0","snacks.txt\0","refreshments.txt\0"};
    struct item s;
    system("cls");

    printf("\t\t\t**********STORE: ALL ITEMS**********\n");    //Same as viewcat() except we go through all the category files
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
    printf("\t|%-6s|%-20s|%-9s|%-11s|%-20s|%-6s|\n","ItemID","Item Name","Price","Qty in stock","Features", "Seller ID");
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");

    FILE *display;
    for(i=0; i<5; i++)
    {
        display = fopen(display_array[i], "r");
        if(display==NULL)
        {
            continue;
        }
        while(fscanf(display, "%6d %20s %f %4d %20s %6d\n", &s.item_id, s.item_name, &s.price, &s.stock_qty, s.features, &s.item_seller_id) != EOF)
        {
            if(s.stock_qty>0)
            {
                printf("\t|%6d|%-20s|%9.2f|   %9d|%-20s|%6d   |\n", s.item_id, s.item_name, s.price, s.stock_qty, s.features, s.item_seller_id);
            }
        }
        fclose(display);
    }
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n\n");
}

void customer_addtocart()
{
    FILE *cart;
    cart = fopen("cart.txt","a+");
    struct item s;
    char cat_array[5][20]={"vegetables.txt\0","dry-fruits.txt\0","fruits.txt\0","snacks.txt\0","refreshments.txt\0"};
    FILE *fp;

    int id, qty, status, i;
    char ch;
    do
    {
        status=0;
        printf("\nEnter Item ID: ");
        scanf("%d", &id);
        printf("Enter Quantity: ");
        scanf("%d", &qty);
        for(i=0; i<5; i++)
            {
                fp = fopen(cat_array[i], "r");
                if(fp==NULL)
                    {
                        continue;
                    }
                while(fscanf(fp, "%6d %20s %f %4d %20s %6d\n", &s.item_id, s.item_name, &s.price, &s.stock_qty, s.features, &s.item_seller_id) != EOF)
                    {
                        if(s.item_id == id && s.stock_qty>=qty)
                        {
                            fprintf(cart, "%6d %20s %9.2f %9d %20s %6d\n", s.item_id, s.item_name, s.price, qty, s.features, s.item_seller_id);
                            status=1;
                            break;
                        }
                    }
            }
        if(status==0)
            printf("\nInvalid ID or quantity.");
        fclose(fp);
        printf("\nDo you want to enter another item?(y/n)\t");
        fflush(stdin);
        scanf("%c", &ch);
        if(ch=='y'||ch=='Y')
            status=0;
    }while(status==0);
    fclose(cart);
}

void customer_checkcart()
{
    int qty;
    char ch;
    struct item s;
    FILE *cart;
    cart = fopen("cart.txt","r");
    fseek(cart, 0, SEEK_SET);

    printf("\n\t\t\t\t**********YOUR CART**********\n");
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
    printf("\t|%-6s|%-20s|%-9s|%-11s |%-20s|%-6s|\n","ItemID","Item Name","Price","Quantity","Features", "Seller ID");
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
    while(fscanf(cart, "%6d %20s %f %4d %20s %6d\n", &s.item_id, s.item_name, &s.price, &qty, s.features, &s.item_seller_id) != EOF)
        {
            printf("\t|%6d|%-20s|%9.2f|   %9d|%-20s|%6d   |\n", s.item_id, s.item_name, s.price, qty, s.features, s.item_seller_id);
        }
    printf("\t+------+--------------------+---------+------------+--------------------+---------+\n");
    fclose(cart);
    printf("Would you like to check out?(y/n)\t");
    fflush(stdin);
    scanf("%c", &ch);
    printf("\n");
    if(ch=='y'||ch=='Y')
        customer_checkout();
}

void customer_checkout()
{
    int qty, pay, stat;
    float subtotal, newtotal=0, total=0;
    char name[20], mode[20], code[15];
    char payment[2][20]={"NetBanking","Cash On Delivery"};
    struct item s;
    FILE *cart, *cdetails, *coupons;

    system("cls");

    cart = fopen("cart.txt","r");
    fseek(cart, 0, SEEK_SET);
    printf("\t\t\t**********BILL OF PURCHASE**********\n");
    printf("\t\t+------+--------------------+---------+-----------+----------+\n");
    printf("\t\t|%-6s|%-20s|%-9s|%-11s|%-10s|\n","ItemID","Item Name","Price","Quantity","Subtotals");
    printf("\t\t+------+--------------------+---------+-----------+----------+\n");
    while(fscanf(cart, "%6d %20s %f %4d %20s %6d\n", &s.item_id, s.item_name, &s.price, &qty, s.features, &s.item_seller_id) != EOF)
        {
            subtotal = s.price*qty;
            total=total+subtotal;
            printf("\t\t|%6d|%-20s|%9.2f|  %9d|%9.2f |\n", s.item_id, s.item_name, s.price, qty, subtotal);
        }
    printf("\t\t+------+--------------------+---------+-----------+----------+\n");
    printf("\t\t|\t\t\t%20s\t  |%9.2f |\n","TOTAL BILL AMOUNT", total);
    printf("\t\t+------+--------------------+---------+-----------+----------+\n");
    newtotal=total;
    if(total==0)
    {
        printf("\n\nCart empty. Please Relog\n");
        exit(0);
    }
    cdetails=fopen("cdetails.txt","a+");

    printf("\nEnter your name:");
    fflush(stdin);
    fgets(name, 19, stdin);
    name[strlen(name)-1] = '\0';

    coupons=fopen("coupons.txt","r");
    while(fscanf(coupons, "%s\n", code) != EOF)
        {
            if(strcmp(code, name)==0)
            {
                printf("\nYou have successfully redeemed %s coupon!",code);
                if(strcmp(code,"HELLOWORLD13")==0 || strcmp(code,"SCOPE144")==0)
                    {
                        printf("\nA special 13%% off on your cart total!");
                        newtotal=0.87*total;
                    }
                else if(strcmp(code,"DBMS20")==0 || strcmp(code,"SEEK_SET4")==0)
                    {
                        printf("\nFor a limited time, 20%% off on your cart total!");
                        newtotal=0.8*total;
                    }
                else if(strcmp(code,"NOTHING")==0)
                    {
                        printf("\nYou have to now pay close to nothing, a big discount!");
                        newtotal=0.1*total;
                    }
                printf("\n\n\t\tTOTAL AMOUNT TO BE PAID: %9.2f", newtotal);
                break;
            }
        }

    if(newtotal!=total)
    {
        printf("\n\nEnter your actual name now:");
        fflush(stdin);
        fgets(name, 19, stdin);
        name[strlen(name)-1] = '\0';
    }

    do{
            stat=0;
            printf("\nChoose a payment method:\n1.NetBanking\n2.Cash on delivery\nChoose:");
            scanf("%d",&pay);
            switch(pay)
            {
                case 1:
                    strcpy(mode, payment[0]);
                    stat=1;
                    break;
                case 2:
                    strcpy(mode, payment[1]);
                    stat=1;
                    break;
                default:
                    printf("\nInvalid payment method. Retry");
            }
    }while(stat==0);

    fprintf(cdetails, "%20s %f %20s\n", name, newtotal, mode);

    printf("\nTHANKS FOR SHOPPING AT OUR WEBSITE");
    printf("\nYou will be redirected to a netbanking site once that is coded.\n\n");
    exit(0);
}





