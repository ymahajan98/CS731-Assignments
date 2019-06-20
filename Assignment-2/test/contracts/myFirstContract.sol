pragma solidity^0.5.0;
contract myFirstContract{
    uint public balance;
    function getBalance() public view returns (uint){
        return balance;
    }
    constructor() public{
        balance=100;
    }
}
