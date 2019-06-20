pragma solidity 0.5.1;
import "browser/Course.sol";

contract Manager {
    //Address of the school administrator
    address admin;
    
    mapping (address => int) student;
    mapping (address => bool) isStudent;
    mapping (int => bool) isCourse;
    mapping (int => Course) course;
    
    int rollCount = 19111000;
    
    //Constructor
    constructor() public{
        admin=msg.sender;
    }
    
    
    function kill() public{
        //The admin has the right to kill the contract at any time.
        //Take care that no one else is able to kill the contract
        require(admin==msg.sender);
        selfdestruct(msg.sender);
    }
    
    function addStudent() public{
        //Anyone on the network can become a student if not one already
        //Remember to assign the new student a unique roll number
        require(isStudent[msg.sender]==false);
        isStudent[msg.sender]=true;   
        student[msg.sender]=rollCount;
        rollCount=rollCount+1; 
    }
    
    function addCourse(int courseNo, address instructor) public{
	//Add a new course with course number as courseNo, and instructor at address instructor
        //Note that only the admin can add a new course. Also, don't create a new course if course already exists
        require(admin==msg.sender);
        require(isCourse[courseNo]==false);
        isCourse[courseNo]=true; 
        course[courseNo]=new Course(courseNo,instructor,admin);
    }
    
    function regCourse(int courseNo) public{
        //Register the student in the course if he is a student on roll and the courseNo is valid
        require(isCourse[courseNo]==true);
        require(isStudent[msg.sender]==true);
        require(course[courseNo].isEnroll(student[msg.sender])==false);
        course[courseNo].enroll(student[msg.sender]);
    }
    
    function getMyMarks(int courseNo) public view returns(int, int, int) {
        //Check the courseNo for validity
        //Should only work for valid students of that course
	//Returns a tuple (midsem, endsem, attendance)
        require(isCourse[courseNo]==true);
        require(isStudent[msg.sender]==true);
        require(course[courseNo].isEnroll(student[msg.sender])==true);
        int mid=course[courseNo].getMidsemMarks(student[msg.sender]);
        int end=course[courseNo].getEndsemMarks(student[msg.sender]);
        int att=course[courseNo].getAttendance(student[msg.sender]);
        return (mid,end,att);
    }
    
    function getMyRollNo() public view returns(int) {
        //Utility function to help a student if he/she forgets the roll number
        //Should only work for valid students
	//Returns roll number as int
        require(isStudent[msg.sender]==true);
        return student[msg.sender];
    }
    
}
