var myFirstContract = artifacts.require("./myFirstContract.sol");
module.exports = function(deployer) {
    deployer.deploy(myFirstContract);
};
