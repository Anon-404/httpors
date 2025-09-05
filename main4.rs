use std::io;

fn main() {
    // Data types
    // NOTE: We have already done int type Data
    // So im skipping intiger

    // Float
    let f1:f32 = 32.123;
    let f2 = 64.123; // NOTE Defulty f64

    println!("f1 is {f1}\nf2 is {f2}");

    // Boolean
    let raining:bool = true;
    let sunlight = false;

    let can_play = raining && sunlight;
    let batter_sleep = raining || sunlight;
    println!("We can play: {}\nBut we can sleep now: {}",can_play,batter_sleep);

    // char
    let ch1:char = 'A';
    let ch2 = '🖕';
    println!("{ch1} : ch1\n{ch2}: ch2");

    // Array
    let arr0:[u8;5] = [10,20,30,40,50];
    let arr1 = [60,70,80,90,100];

    println!("Index num 3 of arr0: {}",arr0[3]);
    println!("Index num 3 of arr1: {}",arr1[3]);
    println!("{:?} : arr0",arr0);
    println!("{:?} : arr1",arr1);


    // Vector
    let mut v0:Vec<i32> = Vec::new();
    // let mut v0 = Vec::<i32>::new(); NOTE: 2nd way to diclaration

    v0.push(1);
    v0.push(2);
    v0.push(3);

    println!("v0 : {:?}",v0);

    let v1 = vec![10,20,30];
    println!("v1 : {:?}",v1);

    // Control flow statement 
    // if else

    let number = 7;
    if number > 10 {
        println!("Number is greater than 10");
    } else if number > 5 {
        println!("Number is greater than 5 but 10 or less");
    } else {
        println!("Number is 5 or less");
    }

    // loop
    // loop{
    //     println!("This is infinite loop");
    // }
    //NOTE:This is infinite loop
    //use break keyword to to prevent infinite loop

    // while loop
    let mut count = 1;
    while count <= 5 {
        println!("{count}: whole loop");
        count+=1;
    }

    // for loop with array
    let array:[u8;3] = [1,2,7];

    for i in &array {
        println!("{}",i);
    }

    // Match
    let num = 2;
    match num {
        1=> println!("This is number 1"),
        2 | 4 => println!("This is number 2 or 4"),
        3=> println!("This is number 3"),
        _=> println!("Out of range")
    }

    // I/O
    // NOTE: Write use std::io; line beggining of your code

    println!("Input your mane");
    let mut name = String::new();
    io::stdin().read_line(&mut name).expect("Error to take input");
    println!("Your name is {name}");
}
